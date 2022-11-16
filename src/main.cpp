#include <bits/stdc++.h>

#include "lib/fm.h"

using namespace std;

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    if (argc != 4) {
        cout << "******************************** [Error] "
                "*******************************"
             << endl;
        cout << "*                                                             "
                "         *"
             << endl;
        cout << "*   [Usage] ./hw2 <input cells>.cells <input nets>.nets "
                "<output>.out   *"
             << endl;
        cout << "*                                                             "
                "         *"
             << endl;
        cout << "******************************** [Error] "
                "*******************************"
             << endl;
        return 1;
    }

    string input_cell_file = argv[1];
    string input_net_file = argv[2];
    string output_file = argv[3];

    FM fm;
    fm.init(input_cell_file, input_net_file);

    fm.partition();

    fm.output(output_file);

    return 0;
}
