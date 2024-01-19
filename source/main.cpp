#include <bits/stdc++.h>
#include "url_data.hpp"
#include "problem.hpp"
#include "useful_functions.hpp"

int main() {
	std::cout << "sign in oj.uz\n";
	if (!oj_sign_in()) {
		return 0;
	}
	Problem problem("JOI20_ho_t1", "oj", 1, 0);
	while (true) {
		std::string command;
		std::cin >> command;
		if (command == "exit") {
			break;
		} else if (command == "submit") {
			std::string filename;
			std::cin >> filename;
			problem.submit(read_from_file(filename));
		}
	}
	write_in_file("temp.sh", "");
	write_in_file("temp.ans", "");
}