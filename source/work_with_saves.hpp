#ifndef WORK_WITH_SAVES_
#define WORK_WITH_SAVES_ 1

#include <fstream>
#include <string>
#include "problem.hpp"

void load_data(const std::string& filename, std::vector<Problem>& data) {
    std::ifstream fin;
    fin.open(filename);
    int sz;
    if (fin >> sz) {
        data.resize(sz);
        for (int i = 0; i < sz; i++) {
            fin >> data[i];
        }
    }
}

void save_data(const std::string& filename, std::vector<Problem>& data) {
    std::ofstream fout;
    fout.open(filename);
    fout << data.size() << '\n';
    for (auto& i : data) {
        fout << i;
    }
}

#endif