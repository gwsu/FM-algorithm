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

const intg max_found = 50;

BucketElement FMMetaData::get_candidate(bool overall) {
    if (overall) {
        for (auto &candidate : b.st) {
            if (is_legal_group_size(candidate.cell_idx))
                return candidate;
        }
        return {};
    }

    intg g0 = 0;
    intg g1 = 0;
    BucketElement c0;
    BucketElement c1;
    for (auto &c : b.st0) {
        if (is_legal_group_size(c.cell_idx)) {
            c0 = c;
            break;
        }
        g0++;
        if (g0 > max_found)
            break;
    }
    for (auto &c : b.st1) {
        if (is_legal_group_size(c.cell_idx)) {
            c1 = c;
            break;
        }
        g1++;
        if (g1 > max_found)
            break;
    }

    if (c0.empty() && c1.empty())
        return {};
    else if (c0.empty())
        return c1;
    else if (c1.empty())
        return c0;
    else
        return (c0.gain_value > c1.gain_value) ? c0 : c1;

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
    b.st.clear();
    for (int i = 0; i < fmptr->num_cell; ++i) {
        calculate_gain(i);
        b.add_element(cell_group[i], i, gain[i]);
    }
}

}  // namespace std
