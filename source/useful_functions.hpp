#ifndef USEFUL_FUNCTIONS_HPP_
#define USEFUL_FUNCTIONS_HPP_ 1

#include <string>
#include <fstream>
#include <iostream>

void replace(std::string& s, const std::string& cut, const std::string& paste) {
	while (s.find(cut) < s.size()) {
		s.replace(s.find(cut), cut.size(), paste);
	}
}

std::string read_from_file(const std::string& filename) {
	std::ifstream fin;
	fin.open(filename, std::ios::binary);
	if (!fin.is_open()) {
		std::cout << filename + " not found\n";
		return "";
	}
	std::string ans, temp;
	while (getline(fin, temp)) {
		ans += temp + '\n';
	}
	fin.close();
	return ans;
}

void write_in_file(const std::string& filename, const std::string& message) {
	std::ofstream fout;
	fout.open(filename);
	if (!fout.is_open()) {
		std::cout << filename + " not found\n";
		return;
	}
	fout << message;
	fout.close();
}

void copy_file(const std::string& original_filename, const std::string& copy_filename) {
	std::ifstream fin;
	std::ofstream fout;
	fin.open(original_filename, std::ios::binary);
	fout.open(copy_filename, std::ios::binary);
	fout << fin.rdbuf();
	fin.close();
	fout.close();
}

std::string string_to_url(const std::string& s) {
	std::string res;
	for (auto i : s) {
		if (i == ' ') {
			res += '+';
		} else if ((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z')) {
			res += i;
		} else {
			int c1 = i / 16, c2 = i % 16;
			res += "%";
			if (c1 < 10) {
				res += std::to_string(c1);
			} else {
				res += char('a' + (c1 - 10));
			}
			if (c2 < 10) {
				res += std::to_string(c2);
			} else {
				res += char('a' + (c2 - 10));
			}
		}
	}
	return res;
}

void run_file(const std::string& filename) {
#ifdef WINDOWS
    std::system(filename.c_str());
#else
    std::system(("./" + filename).c_str());
#endif
}

#endif