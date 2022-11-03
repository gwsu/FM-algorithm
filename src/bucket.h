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
    bool operator<(const BucketElement &rhs) const {
        // NOTE: we want the descending order
        if (cell_idx == rhs.cell_idx) {
            return false;
        } else {
            if (gain_value == rhs.gain_value)
                return cell_idx < rhs.cell_idx;
            else
                return gain_value > rhs.gain_value;
        }
    }
};

class Bucket {
public:
    // set<BucketElement> st0;
    // set<BucketElement> st1;
    // set<BucketElement> st;

    // map<intg, unordered_set<BucketElement>, greater<>> m;
    map<intg, set<intg>, greater<>> m0;
    map<intg, set<intg>, greater<>> m1;

public:
    Bucket() {}
    Bucket(Bucket &rhs) {
        m0 = map<intg, set<intg>, greater<>>(rhs.m0.begin(), rhs.m0.end());
        m1 = map<intg, set<intg>, greater<>>(rhs.m1.begin(), rhs.m1.end());
    }
    void add_element(intg cell_group, intg cell_idx, intg gain_value) {
        if (cell_group == 0) {
            auto m_it = m0.find(gain_value);
            if (m_it == m0.end()) {
                set<intg> us;
                us.insert(cell_idx);
                m0[gain_value] = us;
            } else {
                m0[gain_value].insert(cell_idx);
            }
        } else {
            auto m_it = m1.find(gain_value);
            if (m_it == m1.end()) {
                set<intg> us;
                us.insert(cell_idx);
                m1[gain_value] = us;
            } else {
                m1[gain_value].insert(cell_idx);
            }
        }
    }
    void remove_element(intg cell_group, intg cell_idx, intg gain_value) {
        if (cell_group == 0) {
            auto m_it = m0.find(gain_value);
            if (m_it != m0.end()) {
                if (m_it->second.size() == 0)
                    return;
                else if (m_it->second.size() == 1 &&
                         m_it->second.count(cell_idx))
                    m0.erase(m_it);
                else
                    m_it->second.erase(cell_idx);
            }
        } else {
            auto m_it = m1.find(gain_value);
            if (m_it != m1.end()) {
                if (m_it->second.size() == 0)
                    return;
                else if (m_it->second.size() == 1 &&
                         m_it->second.count(cell_idx))
                    m1.erase(m_it);
                else
                    m_it->second.erase(cell_idx);
            }
        }
    }
};



}  // namespace std

#endif  // BUCKET_H_
