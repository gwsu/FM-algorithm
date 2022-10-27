#ifndef FM_H_
#define FM_H_

#include <string>
#include <unordered_map>

#include "cell.h"
#include "group.h"
#include "net.h"
#include "type.h"

namespace std {

class FM {
public:
    vector<Cell> cell_array;
    vector<Net> net_array;

    unordered_map<string, intg> cell_name_to_cell_idx;

    vector<Group> group_array;

    void initial_partition();
    void fm_partition();

public:
    FM() { group_array = vector<Group>{Group{0}, Group{1}}; }
    void init(string cell_file, string net_file);
    void partition();
    void output(string output_file);
    intg cut_size();
};

}  // namespace std

#endif  // FM_H_
