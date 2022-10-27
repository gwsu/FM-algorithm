#include <iostream>

#include "fm.h"

namespace std {

intg FM::cut_size() {
    intg num_cut = 0;

    for (auto &n : net_array) {
        intg group_num = -100;
        bool cut = false;

        for (auto &c_id : n.cell_indexs) {
            auto &c = cell_array[c_id];
            if (group_num == -100) {
                group_num = c.group_number;
            } else {
                if (group_num != c.group_number) {
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

}  // namespace std
