#ifndef BALANCE_H_
#define BALANCE_H_

#include <numeric>
#include <vector>

#include "fm.h"
#include "group.h"
#include "type.h"

namespace std {

inline intg get_group_size(FM &fm, Group &group) {
    return accumulate(group.cell_index.begin(), group.cell_index.end(), 0,
                      [&](intg tmp_ans, intg c_idx) {
                          return tmp_ans +
                                 fm.cell_array[c_idx].get_size(group.group_idx);
                      });
}

const fp err = 1.0;
inline bool group_valid(intg g0_sz, intg g1_sz, bool fixed = false) {
    return abs(static_cast<fp>(g0_sz) - static_cast<fp>(g1_sz)) <
           ((static_cast<fp>(g0_sz) + static_cast<fp>(g1_sz)) /
            (10.0 - ((fixed) ? err : 0.0)));
}


inline intg greedy_swap_candidate(FM &fm,
                                  Group &g,
                                  vector<bool> &done,
                                  vector<intg> &gain) {
    // NOTE: I use max cell size reduction, if move to the other group.
    return *max_element(g.cell_index.begin(), g.cell_index.end(),
                        [&](const auto &lhs, const auto &rhs) {
                            if (done[lhs] && !done[rhs]) {
                                return true;
                            } else if (!done[lhs] && done[rhs]) {
                                return false;
                            } else {
                                // return
                                // fm.cell_array[lhs].size_reduction(fm.cell_group[lhs])
                                // <
                                //        fm.cell_array[rhs].size_reduction(fm.cell_group[rhs]);
                                return gain[lhs] < gain[rhs];
                                // return
                                // fm.cell_array[lhs].get_size(fm.cell_group[lhs])
                                // <
                                //        fm.cell_array[rhs].get_size(fm.cell_group[rhs]);
                            }
                        });
}



}  // namespace std

#endif  // BALANCE_H_
