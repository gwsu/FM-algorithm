#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>

#include "fm.h"

namespace std {

void FM::init(string cell_file, string net_file) {
    // cell
    fstream cell_fs;
    cell_fs.open(cell_file.c_str(), fstream::in);

    string cell_name;
    intg cell_size1, cell_size2;
    intg counter = 0;
    cell_array.clear();
    while (cell_fs >> cell_name) {
        if (cell_name == "") {
            cout << "Error input cell" << endl;
            exit(141);
        }

        cell_name_to_cell_idx[cell_name] = counter;
        cell_fs >> cell_size1 >> cell_size2;
        cell_array.emplace_back(Cell{
            counter,     // index
            cell_size1,  // cell_size1
            cell_size2,  // cell_size2
            cell_name,   // cell_name
            // TODO: initial cell group number should be what?
            -1,  // group number
        });
        counter++;
    }
    cell_fs.close();
    cell_s_net = vector<vector<intg>>(cell_array.size());
    cell_s_effect = vector<set<intg>>(cell_array.size());

    // net
    fstream net_fs;
    net_fs.open(net_file.c_str(), fstream::in);

    string tmp, net_name, cell_idx;
    counter = 0;
    net_array.clear();
    while (net_fs >> tmp) {
        if (tmp != "NET") {
            cout << "Error input net" << endl;
            exit(141);
        }

        Net n;
        vector<intg> cell_id;
        net_fs >> net_name >> tmp /* { */;
        while (net_fs >> cell_idx) {
            if (cell_idx == "}") {
                break;
            }
            auto idx = cell_name_to_cell_idx[cell_idx];
            cell_id.emplace_back(idx);
            cell_array[idx].net_indexs.emplace_back(counter);
            cell_s_net[idx].emplace_back(counter);
        }
        for (int i = 0; i < cell_id.size(); ++i) {
            for (int j = 0; j < cell_id.size(); ++j) {
                cell_s_effect[cell_id[i]].emplace(cell_id[j]);
            }
        }

        n.index = counter;
        n.net_name = net_name;
        n.number_of_element = cell_id.size();
        n.cell_indexs = cell_id;

        net_array.emplace_back(n);
        counter++;
    }
    net_fs.close();
    cout << "Net : " << counter << endl;

    initial_internal_parameter();
}

}  // namespace std
