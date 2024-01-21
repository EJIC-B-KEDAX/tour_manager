#include <bits/stdc++.h>
#include "work_with_url.hpp"
#include "problem.hpp"
#include "useful_functions.hpp"
#include "work_with_saves.hpp"

int main() {
	std::cout << "sign in oj.uz\n";
	if (!oj_sign_in()) {
		return 0;
	}
	std::vector<Problem> problems, upsolving;
    load_data("save/problems.lox", problems);
    load_data("save/upsolving.lox", upsolving);
	while (true) {
		std::string command;
		std::cin >> command;
		if (command == "exit") {
			break;
		} else if (command == "save") {
            save_data("save/problems.lox", problems);
            save_data("save/upsolving.lox", upsolving);
            std::cout << "saved\n";
        } else if (command == "add_problem_to_upsolving") {
            std::string type, link;
            int complexity;
            std::cin >> type >> link >> complexity;
            upsolving.emplace_back(type, link, complexity, upsolving.size());
            std::cout << "problem index " << upsolving.size() - 1 << '\n';
        } else if (command == "show_statement") {
            std::string type_search, filename;
            std::cin >> type_search;
            if (type_search == "index") {
                int ind;
                std::cin >> ind >> filename;
                if (ind < upsolving.size()) {
                    std::cout << "found\n";
                    copy_file(upsolving[ind].statement_name(), filename);
                } else {
                    std::cout << "wrong index\n";
                }
            } else if (type_search == "name") {
                std::string name, statement_name;
                std::cin >> name >> filename;
                int ind = -1;
                for (int i = 0; i < upsolving.size(); i++) {
                    if (name == upsolving[i].get_name()) {
                        statement_name = upsolving[i].statement_name();
                        ind = i;
                        break;
                    }
                }
                if (ind == -1) {
                    std::cout << "problem not found\n";
                } else {
                    std::cout << "ok, problem index id " << ind << '\n';
                    copy_file(statement_name, filename);
                }
            } else {
                std::cout << "invalid search type\n";
            }
        } else if (command == "submit") {
            int ind;
            std::string filename;
            std::cin >> ind >> filename;
            if (ind >= upsolving.size()) {
                std::cout << "wrong index\n";
                continue;
            }
            std::cout << "sure?\n";
            std::string ans;
            std::cin >> ans;
            if (ans != "no") {
                upsolving[ind].submit(read_from_file(filename));
            } else {
                std::cout << "ok\n";
            }
        } else if (command == "sys") {
            std::string com;
            std::getline(std::cin, com);
            std::system(com.c_str());
        } else {
            std::cout << "invalid command " + command << '\n';
        }
	}
    save_data("save/problems.lox", problems);
    save_data("save/upsolving.lox", upsolving);
	write_in_file("temp.sh", "");
	write_in_file("temp.ans", "");
}