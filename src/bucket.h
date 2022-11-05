#ifndef BUCKET_H_
#define BUCKET_H_

#include <iostream>
#include <map>
#include <set>
#include <unordered_set>

#include "type.h"

namespace std {

class BucketElement {
public:
    intg cell_idx;
    intg gain_value;

public:
    BucketElement() {
        cell_idx = -1;
        gain_value = 0;
    }
    BucketElement(const BucketElement &rhs) {
        cell_idx = rhs.cell_idx;
        gain_value = rhs.gain_value;
    }
    BucketElement(intg _c, intg _g = 0) : cell_idx(_c), gain_value(_g) {}
    bool empty() { return cell_idx == -1; }
};

class Bucket {
public:
    // pair<intg, intg> --> cell size, cell idx
    // cell size from small to large
    map<intg, set<pair<intg, intg>, greater<pair<intg, intg>>>, greater<>> m0;
    map<intg, set<pair<intg, intg>, greater<pair<intg, intg>>>, greater<>> m1;

public:
    Bucket() {}
    Bucket(Bucket &rhs) {
        m0 = rhs.m0;
        m1 = rhs.m1;
    }
    void add_element(intg cell_group,
                     intg cell_idx,
                     intg gain_value,
                     intg cell_prop) {
        if (cell_group == 0) {
            auto m_it = m0.find(gain_value);
            if (m_it == m0.end()) {
                set<pair<intg, intg>, greater<pair<intg, intg>>> us;
                us.insert(make_pair(cell_prop, cell_idx));
                m0[gain_value] = us;
            } else {
                m0[gain_value].insert(make_pair(cell_prop, cell_idx));
            }
        } else {
            auto m_it = m1.find(gain_value);
            if (m_it == m1.end()) {
                set<pair<intg, intg>, greater<pair<intg, intg>>> us;
                us.insert(make_pair(cell_prop, cell_idx));
                m1[gain_value] = us;
            } else {
                m1[gain_value].insert(make_pair(cell_prop, cell_idx));
            }
        }
    }
    void remove_element(intg cell_group,
                        intg cell_idx,
                        intg gain_value,
                        int cell_prop) {
        if (cell_group == 0) {
            auto m_it = m0.find(gain_value);
            if (m_it != m0.end()) {
                if (m_it->second.size() == 0)
                    return;
                else if (m_it->second.size() == 1 &&
                         m_it->second.count(make_pair(cell_prop, cell_idx)))
                    m0.erase(m_it);
                else
                    m_it->second.erase(make_pair(cell_prop, cell_idx));
            }
        } else {
            auto m_it = m1.find(gain_value);
            if (m_it != m1.end()) {
                if (m_it->second.size() == 0)
                    return;
                else if (m_it->second.size() == 1 &&
                         m_it->second.count(make_pair(cell_prop, cell_idx)))
                    m1.erase(m_it);
                else
                    m_it->second.erase(make_pair(cell_prop, cell_idx));
            }
        }
    }
};



}  // namespace std

#endif  // BUCKET_H_
