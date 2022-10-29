#include <assert.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <utility>

#include "balance.h"
#include "bucket.h"
#include "cell.h"
#include "fm.h"
#include "group.h"

const intg INVALID = std::numeric_limits<intg>::min();

namespace std {

void FM::flush_cell_group() {
    group_array[0].cell_index.clear();
    group_array[1].cell_index.clear();
    for (int i = 0; i < num_cell; ++i) {
        cell_array[i].set_group(cell_group[i]);
        group_array[cell_group[i]].add_cell(i);
    }
}

void FM::partition() {
    initial_partition();
    fm_partition();
}

void FM::initial_partition() {
    vector<intg> sorted_cell(num_cell);
    for (int i = 0; i < num_cell; ++i)
        sorted_cell[i] = i;

    sort(
        sorted_cell.begin(), sorted_cell.end(),
        // [&](Cell lhs, Cell rhs) { return lhs.avg_size() < rhs.avg_size(); });
        [&](intg lhs, intg rhs) {
            return cell_array[lhs].max_size() < cell_array[rhs].max_size();
        });
    // [&](Cell lhs, Cell rhs) { return lhs.min_size() > rhs.min_size(); });

    // TODO: For multi-thread version, We can use shuffle to generate different
    // initial solution :
    // shuffle(shuffle_p.begin(), shuffle_p.end(), g);

    // greedy
    intg g0_sz = 0, g1_sz = 0;
    intg group_num = -1;
    for (int i = 0; i < num_cell; ++i) {
        auto c_id = sorted_cell[i];
        auto &c = cell_array[sorted_cell[i]];
        if (g0_sz > g1_sz) {
            group_num = 1;
        } else {
            group_num = 0;
        }

        cell_group[c_id] = group_num;
        group_array[group_num].add_cell(c_id);

        if (group_num == 0) {
            g0_sz += c.get_size(group_num);
        } else {
            g1_sz += c.get_size(group_num);
        }
    }

    intg counter = 0;
    vector<bool> moved(num_cell, false);
    intg from_group = -1;
    intg to_group = -1;
    intg inner_counter = 0;

    while (!group_valid(g0_sz, g1_sz)) {
        inner_counter = 0;
        while (!group_valid(g0_sz, g1_sz)) {
            if (g0_sz > g1_sz) {
                from_group = 0;
                to_group = 1;
            } else {
                from_group = 1;
                to_group = 0;
            }

            auto change_cell =
                greedy_swap_candidate(*this, group_array[from_group], moved);
            moved[change_cell] = true;
            group_array[from_group].remove_cell(change_cell);
            group_array[to_group].add_cell(change_cell);
            cell_group[change_cell] = to_group;

            if (g0_sz > g1_sz) {
                g0_sz -= cell_array[change_cell].get_size(from_group);
                g1_sz += cell_array[change_cell].get_size(to_group);
            } else {
                g1_sz -= cell_array[change_cell].get_size(from_group);
                g0_sz += cell_array[change_cell].get_size(to_group);
            }
            inner_counter++;
            if (inner_counter == num_cell)
                break;

            // TODO: add some rollback to smallest cutsize.
        }
        counter += inner_counter;
    }

    flush_cell_group();
    cout << "Swap : " << counter << " Times." << endl;
    cout << "===================================================" << endl;
    cout << "group 0 size : " << get_group_size(*this, group_array[0]) << endl;
    cout << "group 1 size : " << get_group_size(*this, group_array[1]) << endl;
    cout << "Cut : " << cut_size() << endl;
    cout << "Valid : " << group_valid(g0_sz, g1_sz) << endl;
}
void FM::fm_partition() {
    // calculate initial gain
    FMMetaData now;
    FMMetaData best_fm_data;
    best_fm_data.init(this);

    // FM loop here
    intg best_cost = best_fm_data.get_cut_size();
    intg tmp_cost;

    intg cost_improvement = INVALID;
    // Does this pass improve the cut size?
    bool had_improved = false;
    intg counter = 0;
    double tstart, tend;
    std::chrono::time_point<std::chrono::high_resolution_clock> t_start, t_end;
    while (true) {
        now = best_fm_data;
        now.reset_lock();
        now.reconstruct_bucket();

        had_improved = false;
        tmp_cost = best_cost;
        t_start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < num_cell - 1; ++i) {
            cost_improvement = now.update();
            if (cost_improvement == INVALID)
                break;

            tmp_cost -= cost_improvement;
            if (best_cost > tmp_cost) {
                best_cost = tmp_cost;
                best_fm_data = now;
                had_improved = true;
            }
        }
        t_end = std::chrono::high_resolution_clock::now();
        cout << "Best cost: " << best_cost << ", "
             << std::chrono::duration<double, std::milli>(t_end - t_start)
                    .count()
             << " millisecond" << endl;

        if (!had_improved) {
            break;
        }
    }
    cell_group = best_fm_data.cell_group;
    flush_cell_group();
    cout << "=======================================" << endl;
    cout << "group 0 size : " << get_group_size(*this, group_array[0]) << endl;
    cout << "group 1 size : " << get_group_size(*this, group_array[1]) << endl;
    cout << "Cut : " << cut_size() << endl;
}

intg FMMetaData::update() {
    auto &fm = *fmptr;
    // find best gain candidate
    BucketElement cell_id_gain_value = get_candidate();

    if (cell_id_gain_value.empty())
        return INVALID;

    auto &c_id = cell_id_gain_value.cell_idx;
    auto &c_gain = cell_id_gain_value.gain_value;

    // mark cell
    done[c_id] = true;

    // move cell
    intg from_group = -1;
    intg to_group = -1;
    if (cell_group[c_id] == 0) {
        from_group = 0;
        to_group = 1;
        g0_sz -= fm.cell_array[c_id].get_size(0);
        g1_sz += fm.cell_array[c_id].get_size(1);
    } else {
        from_group = 1;
        to_group = 0;
        g1_sz -= fm.cell_array[c_id].get_size(1);
        g0_sz += fm.cell_array[c_id].get_size(0);
    }
    cell_group[c_id] = to_group;

    // update hyper_part
    b.remove_element(c_id, gain[c_id]);
    for (auto &n : fm.cell_s_net[c_id]) {
        if (from_group == 0)
            hyperedge_part[n]--;
        else
            hyperedge_part[n]++;
    }

    // update gain
    for (auto &effect_c : fm.cell_s_effect[c_id]) {
        if (!done[effect_c]) {
            b.remove_element(effect_c, gain[effect_c]);
            calculate_gain(effect_c);
            b.add_element(effect_c, gain[effect_c]);
        }
    }


    return c_gain;
}

}  // namespace std
