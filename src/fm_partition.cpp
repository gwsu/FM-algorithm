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
const double time_limit = 200;

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
    // greedy
    intg g0_sz = 0, g1_sz = 0;
    intg group_num = -1;
    for (intg i = 0; i < num_cell; ++i) {
        auto c_id = i;
        auto &c = cell_array[i];
        group_num = 0;

        cell_group[c_id] = group_num;
        group_array[group_num].add_cell(c_id);

        if (group_num == 0) {
            g0_sz += c.get_size(group_num);
        } else {
            g1_sz += c.get_size(group_num);
        }
    }

    refinement(g0_sz, g1_sz);

    flush_cell_group();
#ifdef DEBUG
    cout << "===================================================" << endl;
    cout << "group 0 size : " << get_group_size(*this, group_array[0]) << endl;
    cout << "group 1 size : " << get_group_size(*this, group_array[1]) << endl;
    cout << "Valid : " << group_valid(g0_sz, g1_sz) << endl;
#endif
    cout << "Cut : " << cut_size() << endl;
}


void FM::refinement(intg g0_sz, intg g1_sz) {
    FMMetaData for_gain_calculation;
    vector<intg> _gain(num_cell, 0);
    for_gain_calculation.only_gain(_gain, this);

    intg counter = 0;
    vector<bool> moved(num_cell, false);
    intg from_group = -1;
    intg to_group = -1;
    // size, cell_idx
    set<pair<intg, intg>, greater<>> s0;
    set<pair<intg, intg>, greater<>> s1;
    s0.clear();
    s1.clear();
    for (int i = 0; i < num_cell; ++i) {
        if (cell_group[i] == 0) {
            s0.insert(make_pair(cell_array[i].sz0, i));
        } else {
            s1.insert(make_pair(cell_array[i].sz0, i));
        }
    }

    while (!group_valid(g0_sz, g1_sz)) {
        intg change_cell = -1;
        if (g0_sz > g1_sz) {
            from_group = 0;
            to_group = 1;
            auto cand = s0.begin();
            change_cell = cand->second;
            s0.erase(cand);
        } else {
            from_group = 1;
            to_group = 0;
            auto cand = s1.begin();
            change_cell = cand->second;
            s1.erase(cand);
        }

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
        counter++;
        if (counter == num_cell)
            break;
    }
#ifdef DEBUG
    cout << "Refine : " << counter << " Times." << endl;
#endif
}


void FM::fm_partition() {
    // calculate initial gain
    FMMetaData best_fm_data;
    best_fm_data.init(this);
    FMMetaData now = best_fm_data;

    // FM loop here
    intg best_cost = best_fm_data.get_cut_size();
    intg tmp_cost;
    intg prev_cost;

    intg cost_improvement = INVALID;
    // Does this pass improve the cut size?
    bool had_improved = false;
#ifdef DEBUG
    std::chrono::time_point<std::chrono::high_resolution_clock> t_start, t_end;
#endif

    intg epoch = 0;
    vector<intg> record;
    record.reserve(num_cell);
    while (true) {
        now.copy(best_fm_data);
        now.reset_lock();
        now.reconstruct_bucket();

        had_improved = false;
        tmp_cost = best_cost;
        prev_cost = best_cost;
        record.clear();
#ifdef DEBUG
        t_start = std::chrono::high_resolution_clock::now();
#endif
        // NOTE: based on my experiment
        // for pass 1, when num_cell/2 > i > (num_cell/2)/2 will optimize the
        // solution.
        // for pass 2, only i < (num_cell/2)/10 will optimize the solution.
        // for pass 3, 4, 5... are more less times to optimize the solution.
        intg half_cell = (num_cell / 2) / ((epoch == 0) ? 1 : 10);
        // NOTE: based on my experiment, when i > 505 of cell, it won't optimize
        // the best_cost anymore.
        for (int i = 0; i < half_cell; ++i) {
            cost_improvement = now.update(global_start, -1, record);

            // NOTE: based on my experiment, when i > 75% of cell, it couldn't
            // find the suitable candidate and will break this pass, even at the
            // first pass.
            if (cost_improvement == INVALID)
                break;

            tmp_cost -= cost_improvement;
            if (best_cost > tmp_cost) {
                best_cost = tmp_cost;
                had_improved = true;
            }
        }



#ifdef DEBUG
        t_end = std::chrono::high_resolution_clock::now();

        cout << "Best cost: " << best_cost << ", "
             << std::chrono::duration<double, std::milli>(t_end - t_start)
                    .count()
             << " millisecond" << endl;
#else
        cout << "Best cost: " << best_cost << endl;
#endif

        global_end = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::ratio<1, 1>>(
                              global_end - global_start)
                              .count();

        if (!had_improved) {
            break;
        }

        // replay
        tmp_cost = prev_cost;
        best_fm_data.reset_lock();
        best_fm_data.reconstruct_bucket();
        for (int i = 0; i < half_cell; ++i) {
            cost_improvement = best_fm_data.update(global_end, i, record);
            tmp_cost -= cost_improvement;
            if (tmp_cost <= best_cost)
                break;
        }

        if (duration > time_limit) {
            cout << "Time out break" << endl;
            break;
        }
        epoch++;
    }
    cell_group = best_fm_data.cell_group;
    flush_cell_group();
    intg g0_sz = best_fm_data.g0_sz;
    intg g1_sz = best_fm_data.g1_sz;

    if (!group_valid(g0_sz, g1_sz)) {
        refinement(g0_sz, g1_sz);
        flush_cell_group();
#ifndef DEBUG
        cout << "Cut : " << cut_size() << endl;
#endif
    }

#ifdef DEBUG
    cout << "=======================================" << endl;
    cout << "group 0 size : " << get_group_size(*this, group_array[0]) << endl;
    cout << "group 1 size : " << get_group_size(*this, group_array[1]) << endl;
    cout << "Cut : " << cut_size() << endl;
#endif
}

intg FMMetaData::update(
    std::chrono::time_point<std::chrono::high_resolution_clock> start,
    intg mode,
    vector<intg> &record) {
    auto &fm = *fmptr;

    auto end = std::chrono::high_resolution_clock::now();
    double duration =
        std::chrono::duration<double, std::ratio<1, 1>>(end - start).count();
    if (duration > time_limit) {
        return INVALID;
    }

    // find best gain candidate
    BucketElement cell_id_gain_value;
    if (mode == -1) {
        cell_id_gain_value = get_candidate();
        record.emplace_back(cell_id_gain_value.cell_idx);
    } else {
        cell_id_gain_value = BucketElement{record[mode], gain[record[mode]]};
    }

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
    b.remove_element(from_group, c_id, gain[c_id],
                     fm.cell_array[c_id].get_size(to_group));
    for (auto &n : fm.cell_s_net[c_id]) {
        if (from_group == 0)
            hyperedge_part[n]--;
        else
            hyperedge_part[n]++;
    }

    // update gain
    for (auto &effect_c : fm.cell_s_effect[c_id]) {
        if (!done[effect_c]) {
            b.remove_element(
                cell_group[effect_c], effect_c, gain[effect_c],
                fm.cell_array[effect_c].get_size(!cell_group[effect_c]));
            calculate_gain(effect_c);
            b.add_element(
                cell_group[effect_c], effect_c, gain[effect_c],
                fm.cell_array[effect_c].get_size(!cell_group[effect_c]));
        }
    }


    return c_gain;
}

}  // namespace std
