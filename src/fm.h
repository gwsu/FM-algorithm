#ifndef FM_H_
#define FM_H_

#include <string>
#include <unordered_map>
#include <utility>  // std::pair, std::make_pair

#include "bucket.h"
#include "cell.h"
#include "group.h"
#include "net.h"
#include "type.h"

namespace std {

class FM {
public:
    intg num_cell;
    intg num_net;
    vector<Cell> cell_array;
    vector<Net> net_array;
    vector<vector<intg>> cell_s_net;
    vector<vector<intg>> cell_s_effect;

    vector<intg> cell_group;

    unordered_map<string, intg> cell_name_to_cell_idx;

    vector<Group> group_array;

    void initial_internal_parameter() {
        num_cell = cell_array.size();
        num_net = net_array.size();
        cell_group = vector<intg>(num_cell, 0);
    }

    void initial_partition();
    void fm_partition();

public:
    FM() { group_array = vector<Group>{Group{0}, Group{1}}; }
    void init(string cell_file, string net_file);
    void partition();
    void output(string output_file);
    intg cut_size();
    void flush_cell_group();
};

class FMMetaData {
public:
    // accelerate the search of (max/min) gain value
    Bucket b;

    // number of nodes in the nets that are in the group0
    vector<intg> hyperedge_part;

    // gain value
    vector<intg> gain;

    // cell_group
    vector<intg> cell_group;

    intg g0_sz;
    intg g1_sz;

    vector<bool> done;

    FM *fmptr;

    BucketElement get_candidate();
    bool is_legal_group_size(intg c_id);

public:
    FMMetaData() {}
    FMMetaData(FMMetaData &rhs) {
        b = rhs.b;
        hyperedge_part = rhs.hyperedge_part;
        gain = rhs.gain;
        cell_group = rhs.cell_group;
        g0_sz = rhs.g0_sz;
        g1_sz = rhs.g1_sz;
        done = rhs.done;
        fmptr = rhs.fmptr;
    }
    void init(FM *_fm);
    void calculate_gain(intg c_id);
    void reset_lock();

    // NOTE: Only for fm loop usage, if you want final answer, please call
    // flush_cell_group() and then call cut_size();
    intg get_cut_size();

    intg update();
};  // namespace std

}  // namespace std

#endif  // FM_H_
