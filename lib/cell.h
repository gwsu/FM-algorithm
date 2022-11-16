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
    intg sz0;
    intg sz1;
    intg group_number;
    vector<intg> net_indexs;
    string cell_name;

public:
    Cell() : index(-1), group_number(-1) {}
    Cell(intg _index, intg _sz1, intg _sz2, string _name, intg _group_number)
        : index(_index),
          sz(vector<intg>{_sz1, _sz2}),
          group_number(_group_number),
          cell_name(_name) {
        sz0 = sz[0];
        sz1 = sz[1];
    }
    inline intg get_size(intg x) { return (x == 0) ? sz0 : sz1; }
    void set_group(intg g) { group_number = g; }
};

}  // namespace std



#endif  // CELL_H_
