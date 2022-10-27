#ifndef GROUP_H_
#define GROUP_H_

#include <algorithm>
#include <numeric>
#include <unordered_set>

#include "type.h"

namespace std {

class Group {
public:
    // intg remain_size;
    unordered_set<intg> cell_index;
    intg group_idx;

public:
    Group(intg _idx) : group_idx(_idx) {}
    void add_cell(intg c) { cell_index.emplace(c); }
    void remove_cell(intg c) { cell_index.erase(c); }
};


}  // namespace std

#endif  // GROUP_H_
