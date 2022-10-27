#include <algorithm>
#include <iostream>
#include <numeric>

#include "balance.h"
#include "cell.h"
#include "fm.h"
#include "group.h"

namespace std {

void FM::partition() {
    initial_partition();
    fm_partition();
}

void FM::initial_partition() {
    vector<intg> sorted_cell(cell_array.size());
    for (int i = 0; i < sorted_cell.size(); ++i)
        sorted_cell[i] = i;

    sort(
        sorted_cell.begin(), sorted_cell.end(),
        // [&](Cell lhs, Cell rhs) { return lhs.avg_size() < rhs.avg_size(); });
        [&](intg lhs, intg rhs) {
            return cell_array[lhs].max_size() < cell_array[rhs].max_size();
        });
    // [&](Cell lhs, Cell rhs) { return lhs.min_size() > rhs.min_size(); });


    // greedy
    intg g0_sz = 0, g1_sz = 0;
    intg group_num = -1;
    for (int i = 0; i < sorted_cell.size(); ++i) {
        auto &c = cell_array[sorted_cell[i]];
        if (g0_sz > g1_sz) {
            group_num = 1;
        } else {
            group_num = 0;
        }

        c.set_group(group_num);
        group_array[group_num].add_cell(c.index);

        if (group_num == 0) {
            g0_sz += c.current_size();
        } else {
            g1_sz += c.current_size();
        }
    }

    intg counter = 0;
    vector<bool> moved(cell_array.size(), false);
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
            cell_array[change_cell].set_group(to_group);

            if (g0_sz > g1_sz) {
                g0_sz -= cell_array[change_cell].get_size(from_group);
                g1_sz += cell_array[change_cell].get_size(to_group);
            } else {
                g1_sz -= cell_array[change_cell].get_size(from_group);
                g0_sz += cell_array[change_cell].get_size(to_group);
            }
            inner_counter++;
            if (inner_counter == cell_array.size())
                break;

            // TODO: add some rollback to smallest cutsize.
        }
        counter += inner_counter;
    }
    cout << "Swap : " << counter << " Times." << endl;
    cout << "===================================================" << endl;
    cout << "group 0 size : " << get_group_size(*this, group_array[0]) << endl;
    cout << "group 1 size : " << get_group_size(*this, group_array[1]) << endl;
    cout << "Cut : " << cut_size() << endl;
    cout << "Valid : " << group_valid(g0_sz, g1_sz) << endl;
}
void FM::fm_partition() {
    // Do nothing right now
}

}  // namespace std
