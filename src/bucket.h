#ifndef BUCKET_H_
#define BUCKET_H_

#include <iostream>
#include <set>

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
    set<BucketElement> st;

public:
    Bucket() {}
    Bucket(Bucket &rhs) {
        st = set<BucketElement>(rhs.st.begin(), rhs.st.end());
    }
    void add_element(intg cell_idx, intg gain_value) {
        st.emplace(BucketElement{cell_idx, gain_value});
    }
    void remove_element(intg cell_idx, intg gain_value) {
        st.erase(BucketElement{cell_idx, gain_value});
    }
};



}  // namespace std

#endif  // BUCKET_H_
