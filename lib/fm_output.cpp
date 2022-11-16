#include <fstream>
#include <iostream>

#include "balance.h"
#include "fm.h"


namespace std {

void FM::output(string output_file) {
    auto io_start = std::chrono::high_resolution_clock::now();
    fstream out;
    out.open(output_file.c_str(), fstream::out);

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
    auto io_end = std::chrono::high_resolution_clock::now();
    cout << "============= IO time ===============" << endl;
    cout << "IO time: "
         << std::chrono::duration<double, std::ratio<1, 1>>(io_end - io_start)
                .count()
         << " s" << endl;
}


}  // namespace std
