#include <fstream>
#include <iostream>

#include "balance.h"
#include "fm.h"


namespace std {

void FM::output(string output_file) {
    fstream out;
    out.open(output_file.c_str(), fstream::out);

    // FIXME: calculate true cut.
    out << "cut_size " << cut_size() << endl;

    for (auto &g : group_array) {
        if (g.group_idx == 0) {
            out << "A " << g.cell_index.size() << endl;
        } else {
            out << "B " << g.cell_index.size() << endl;
        }

        for (auto &c : g.cell_index) {
            out << cell_array[c].cell_name << endl;
        }
    }
}


}  // namespace std
