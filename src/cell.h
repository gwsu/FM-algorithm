#ifndef CELL_H_
#define CELL_H_

#include <string>
#include <vector>

#include "type.h"

namespace std {

class Cell {
public:
    intg index;
    vector<intg> sz;
    intg group_number;
    vector<intg> net_indexs;
    intg size_reduction_meta_ans;
    string cell_name;

public:
    Cell() : index(-1), group_number(-1) {}
    Cell(int _index, int _sz1, int _sz2, string _name, int _group_number)
        : index(_index),
          sz(vector<intg>{_sz1, _sz2}),
          group_number(_group_number),
          cell_name(_name) {
        size_reduction_meta_ans = sz[0] - sz[1];
    }
    intg get_size(intg x) { return sz[x]; }
    intg avg_size() { return (sz[0] + sz[1]) / 2; }
    intg max_size() { return max(sz[0], sz[1]); }
    intg min_size() { return min(sz[0], sz[1]); }
    intg max_group() { return (max_size() == sz[0]) ? 0 : 1; }
    intg min_group() { return (min_size() == sz[0]) ? 0 : 1; }
    intg current_size() { return sz[group_number]; }
    intg size_reduction(intg current_g) {
        return (size_reduction_meta_ans * ((current_g == 0) ? 1 : -1));
    }
    void set_group(intg g) { group_number = g; }
};

}  // namespace std



#endif  // CELL_H_
