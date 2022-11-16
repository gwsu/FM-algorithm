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
inline bool group_valid(intg g0_sz, intg g1_sz) {
    // return abs(static_cast<fp>(g0_sz) - static_cast<fp>(g1_sz)) <
    //        ((static_cast<fp>(g0_sz) + static_cast<fp>(g1_sz)) /
    //         (10.0 - ((fixed) ? err : 0.0)));
    return (g0_sz >= g1_sz) ? (9 * g0_sz < 11 * g1_sz)
                            : (9 * g1_sz < 11 * g0_sz);
}

}  // namespace std

#endif  // BALANCE_H_
