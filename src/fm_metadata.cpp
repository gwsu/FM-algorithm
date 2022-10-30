#include <iterator>

#include "balance.h"
#include "bucket.h"
#include "fm.h"

namespace std {

void FMMetaData::init(FM *_fm) {
    fmptr = _fm;
    auto &fm = *fmptr;
    hyperedge_part = vector<intg>(fm.num_net, 0);
    gain = vector<intg>(fm.num_cell, 0);
    done = vector<bool>(fm.num_cell, false);
    cell_group = fm.cell_group;
    g0_sz = get_group_size(fm, fm.group_array[0]);
    g1_sz = get_group_size(fm, fm.group_array[1]);

    // hyper part
    for (int i = 0; i < fm.num_cell; ++i) {
        if (fm.cell_group[i] == 0) {
            for (const auto &n : fm.cell_s_net[i]) {
                hyperedge_part[n]++;
            }
        }
    }

    // calculate gain
    for (int i = 0; i < fm.num_cell; ++i) {
        calculate_gain(i);
        b.add_element(cell_group[i], i, gain[i]);
    }
}

void FMMetaData::calculate_gain(intg c_id) {
    auto &fm = *fmptr;
    gain[c_id] = 0;
    for (const auto &s : fm.cell_s_net[c_id]) {
        if ((cell_group[c_id] == 0 && hyperedge_part[s] == 1) ||
            (cell_group[c_id] == 1 &&
             hyperedge_part[s] == fm.net_array[s].number_of_element - 1)) {
            gain[c_id]++;
        } else if ((cell_group[c_id] == 0 &&
                    hyperedge_part[s] == fm.net_array[s].number_of_element) ||
                   (cell_group[c_id] == 1 && hyperedge_part[s] == 0)) {
            gain[c_id]--;
        }
    }
}

intg FMMetaData::get_cut_size() {
    auto &fm = *fmptr;
    intg num_cut = 0;

    for (auto &n : fm.net_array) {
        intg group_num = -100;
        bool cut = false;

        for (auto &c_id : n.cell_indexs) {
            if (group_num == -100) {
                group_num = cell_group[c_id];
            } else {
                if (group_num != cell_group[c_id]) {
                    cut = true;
                    break;
                }
            }
        }

        if (cut) {
            num_cut++;
        }
    }
    return num_cut;
}

BucketElement FMMetaData::get_candidate() {
    auto g0_it = b.st0.begin();
    auto g1_it = b.st1.begin();
    for (int i = 0; i < 10; ++i) {
        if (g0_it == b.st0.end() && g1_it == b.st1.end()) {
            return {};
        } else if (g0_it == b.st0.end()) {
            if (is_legal_group_size((*g1_it).cell_idx)) {
                return *g1_it;
            } else {
                g1_it++;
                continue;
            }
        } else if (g1_it == b.st1.end()) {
            if (is_legal_group_size((*g0_it).cell_idx)) {
                return *g0_it;
            } else {
                g0_it++;
                continue;
            }
        }
        auto &g0 = *g0_it;
        auto &g1 = *g1_it;
        if (g0.gain_value > g1.gain_value) {
            if (is_legal_group_size(g0.cell_idx)) {
                return g0;
            } else if (is_legal_group_size(g1.cell_idx)) {
                return g1;
            } else {
                g0_it++;
                g1_it++;
            }
        } else {
            if (is_legal_group_size(g1.cell_idx)) {
                return g1;
            } else if (is_legal_group_size(g0.cell_idx)) {
                return g0;
            } else {
                g0_it++;
                g1_it++;
            }
        }
    }
    return {};
}

bool FMMetaData::is_legal_group_size(intg c_id) {
    auto &c = fmptr->cell_array[c_id];
    intg new_g0_sz = g0_sz;
    intg new_g1_sz = g1_sz;
    if (cell_group[c_id] == 0) {
        new_g0_sz -= c.sz0;
        new_g1_sz += c.sz1;
    } else {
        new_g1_sz -= c.sz1;
        new_g0_sz += c.sz0;
    }
    return group_valid(new_g0_sz, new_g1_sz /* , true */);
}

void FMMetaData::reset_lock() {
    done = vector<bool>(fmptr->num_cell, false);
}

void FMMetaData::reconstruct_bucket() {
    b.st0.clear();
    b.st1.clear();
    for (int i = 0; i < fmptr->num_cell; ++i) {
        calculate_gain(i);
        b.add_element(cell_group[i], i, gain[i]);
    }
}

}  // namespace std
