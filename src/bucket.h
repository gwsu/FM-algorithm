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
    map<intg, set<intg>, greater<>> m;

public:
    Bucket() {}
    Bucket(Bucket &rhs) {
        // st0 = set<BucketElement>(rhs.st0.begin(), rhs.st0.end());
        // st1 = set<BucketElement>(rhs.st1.begin(), rhs.st1.end());
        // st = set<BucketElement>(rhs.st.begin(), rhs.st.end());
        m = map<intg, set<intg>, greater<>>(rhs.m.begin(), rhs.m.end());
    }
    void add_element(intg cell_group, intg cell_idx, intg gain_value) {
        auto m_it = m.find(gain_value);
        if (m_it == m.end()) {
            set<intg> us;
            us.insert(cell_idx);
            m[gain_value] = us;
        } else {
            m[gain_value].insert(cell_idx);
        }
    }
    void remove_element(intg cell_group, intg cell_idx, intg gain_value) {
        auto m_it = m.find(gain_value);
        if (m_it != m.end()) {
            if (m_it->second.size() == 0)
                return;
            else if (m_it->second.size() == 1 && m_it->second.count(cell_idx))
                m.erase(m_it);
            else
                m_it->second.erase(cell_idx);
        }
    }
};



}  // namespace std

#endif  // BUCKET_H_
