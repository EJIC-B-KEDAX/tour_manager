#include <bits/stdc++.h>
#include "url_data.hpp"
#include "problem.hpp"
#include "useful_functions.hpp"

int main() {
	std::cout << "sign in oj.uz\n";
	if (!oj_sign_in()) {
		return 0;
	}
	std::vector<Problem> problems, upsolving;
	while (true) {
		std::string command;
		std::cin >> command;
		if (command == "exit") {
			break;
		}
	}
	write_in_file("temp.sh", "");
	write_in_file("temp.ans", "");
}