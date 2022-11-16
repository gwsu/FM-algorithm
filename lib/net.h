#ifndef NET_H_
#define NET_H_

#include <string>
#include <vector>

#include "type.h"

namespace std {

class Net {
public:
    intg index;
    intg number_of_element;  // number of the element in this net
    intg net_size;           // sum of the cell area
    string net_name;
    vector<intg> cell_indexs;

public:
    Net() : index(-1), number_of_element(-1), net_size(-1) {}
    Net(int _index, int _number_of_element, int _net_size)
        : index(_index),
          number_of_element(_number_of_element),
          net_size(_net_size) {}
    bool operator<(const Net &rhs) const {
        // TODO: dynamic net size here
        return net_size < rhs.net_size;
    }
};


}  // namespace std

#endif  // NET_H_
