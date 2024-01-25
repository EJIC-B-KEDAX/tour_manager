#include <bits/stdc++.h>
#include "work_with_url.hpp"
#include "problem.hpp"
#include "useful_functions.hpp"
#include "work_with_saves.hpp"

std::mt19937 mt(time(nullptr));

int main() {
	std::cout << "sign in oj.uz\n";
	if (!oj_sign_in()) {
		return 0;
	}
	std::vector<Problem> problems, upsolving;
    std::vector<std::vector<Problem>> tour;
    load_data("save/problems.lox", problems);
    load_data("save/upsolving.lox", upsolving);
    load_data("save/tours.lox", tour);
    int start_time = 0, have_time = 0, now_tour = -1;
	while (true) {
		std::string command;
		std::cin >> command;
        if (now_tour != -1 && (clock() - start_time) / CLOCKS_PER_SEC > have_time) {
            std::cout << "summary:\n";
            int sum = 0;
            for (int i = 0; i < tour[now_tour].size(); i++) {
                std::cout << char('A' + i) << ": score " << tour[now_tour][i].get_score() << ", submissions " << tour[now_tour][i].number_of_submissions() << '\n';
                sum += tour[now_tour][i].get_score();
            }
            std::cout << "sum " << sum << "\ntour ended\n";
            tour.erase(tour.begin() + now_tour);
            now_tour = -1;
            start_time = 0;
            have_time = 0;
        }
		if (command == "exit" || command == ":e") {
			break;
		} else if (command == "oj_sign_in") {
            oj_sign_in();
        } else if (command == "save") {
            save_data("save/problems.lox", problems);
            save_data("save/upsolving.lox", upsolving);
            save_data("save/tours.lox", tour);
            std::cout << "saved\n";
        } else if (command == "add_problem_to_upsolving" || command == ":au") {
            std::string type, link;
            int complexity;
            std::cin >> type >> link >> complexity;
            upsolving.emplace_back(type, link, complexity, upsolving.size());
            std::cout << "problem index " << upsolving.size() - 1 << '\n';
        } else if (command == "add_problem_to_base" || command == ":ab") {
            std::string type, link;
            int complexity;
            std::cin >> type >> link >> complexity;
            problems.emplace_back(type, link, complexity, problems.size());
        } else if (command == "search") {
            std::string type_search;
            std::cin >> type_search;
            if (type_search == "name" || type_search == "-n") {
                std::string name;
                std::cin >> name;
                int ind = -1;
                for (int i = 0; i < upsolving.size(); i++) {
                    if (name == upsolving[i].get_name()) {
                        ind = i;
                        break;
                    }
                }
                if (ind == -1) {
                    std::cout << "problem not found\n";
                } else {
                    std::cout << "ok, problem index is " << ind << '\n';
                }
            } else {
                std::cout << "invalid search type\n";
            }
        } else if (command == "show_statement" || command == ":sh") {
            std::string filename;
            int ind;
            std::cin >> ind >> filename;
            if (ind < upsolving.size()) {
                std::cout << "found " << upsolving[ind].get_name() << '\n';
                copy_file(upsolving[ind].statement_name(), filename);
            } else {
                std::cout << "wrong index\n";
            }
        } else if (command == "submit" || command == ":s") {
            int ind;
            std::string filename, ind1;
            std::cin >> ind1 >> filename;
            if (now_tour != -1) {
                ind = ind1[0] - 'A';
                if (ind >= tour[now_tour].size() || ind < 0) {
                    std::cout << "wrong problem\n";
                    continue;
                }
                std::cout << "sure?\n";
                std::string ans;
                std::cin >> ans;
                if (ans != "no") {
                    int old_sz = tour[now_tour][ind].number_of_submissions();
                    tour[now_tour][ind].submit(read_from_file(filename));
                    if (old_sz != tour[now_tour][ind].number_of_submissions()) {
                        while (!tour[now_tour][ind].submission_info(tour[now_tour][ind].number_of_submissions() - 1)) {
                            _sleep(5000);
                        }
                    }
                } else {
                    std::cout << "ok\n";
                }
            } else {
                ind = stoi(ind1);
                if (ind >= upsolving.size()) {
                    std::cout << "wrong index\n";
                    continue;
                }
                std::cout << "sure?\n";
                std::string ans;
                std::cin >> ans;
                if (ans != "no") {
                    int old_sz = upsolving[ind].number_of_submissions();
                    upsolving[ind].submit(read_from_file(filename));
                    if (old_sz != upsolving[ind].number_of_submissions()) {
                        while (!upsolving[ind].submission_info(upsolving[ind].number_of_submissions() - 1)) {
                            _sleep(5000);
                        }
                    }
                } else {
                    std::cout << "ok\n";
                }
            }
        } else if (command == "check_submission" || command == ":c") {
            int ind, submission_id;
            std::string ind1;
            std::cin >> ind1 >> submission_id;
            if (now_tour != -1) {
                ind = ind1[0] - 'A';
                if (ind >= tour[now_tour].size() || ind < 0) {
                    std::cout << "wrong problem\n";
                    continue;
                }
                tour[now_tour][ind].submission_info(submission_id);
            } else {
                ind = stoi(ind1);
                if (ind >= upsolving.size()) {
                    std::cout << "wrong index\n";
                    continue;
                }
                upsolving[ind].submission_info(submission_id);
            }
        } else if (command == "problem_info" || command == ":i") {
            int ind;
            std::string ind1;
            std::cin >> ind1;
            if (now_tour != -1) {
                ind = ind1[0] - 'A';
                if (ind >= tour[now_tour].size() || ind < 0) {
                    std::cout << "wrong problem\n";
                    continue;
                }
                std::cout << ind1 << ": score " << tour[now_tour][ind].get_score() << ", time limit " << tour[now_tour][ind].get_time_limit() << " ms, memory limit " << tour[now_tour][ind].get_memory_limit() << " Mb, submissions " << tour[now_tour][ind].number_of_submissions() << '\n';
            } else {
                ind = stoi(ind1);
                if (ind >= upsolving.size()) {
                    std::cout << "wrong index\n";
                    continue;
                }
                std::cout << upsolving[ind].get_name() << ": score " << upsolving[ind].get_score() << ", time limit " << upsolving[ind].get_time_limit() << " ms, memory limit " << upsolving[ind].get_memory_limit() << " Mb, submissions " << upsolving[ind].number_of_submissions() << '\n';
            }
        } else if (command == "sys") {
            std::string com;
            std::getline(std::cin, com);
            std::system(com.c_str());
        } else if (command == "create_tour") {
            std::string opt;
            std::cin >> opt;
            if (opt == "-r") {
                int sz;
                std::cin >> sz;
                if (problems.size() < sz) {
                    std::cout << "few problems\n";
                    continue;
                }
                tour.emplace_back();
                for (int i = 0; i < sz; i++) {
                    int ind = (int)mt() % (int)problems.size();
                    tour.back().push_back(problems[ind]);
                    problems.erase(problems.begin() + ind);
                }
                std::cout << "tour created, id = " << tour.size() - 1 << '\n';
            } else if (opt == "-c") {
                int sz, ok = 1;
                std::cin >> sz;
                tour.emplace_back();
                shuffle(problems.begin(), problems.end(), mt);
                for (int i = 0; i < sz; i++) {
                    int comp, ind = -1;
                    std::cin >> comp;
                    for (int j = 0; j < problems.size(); j++) {
                        if (problems[j].get_complexity() == comp) {
                            ind = j;
                            break;
                        }
                    }
                    if (ind == -1) {
                        std::cout << "few problems with complexity " << comp << '\n';
                        ok = 0;
                        tour.pop_back();
                        break;
                    }
                    tour.back().push_back(problems[ind]);
                    problems.erase(problems.begin() + ind);
                }
                if (ok) {
                    std::cout << "tour created, id = " << tour.size() - 1 << '\n';
                } else {
                    shuffle(tour.back().begin(), tour.back().end(), mt);
                }
            } else if (opt == "-fc") {
                int sz;
                std::cin >> sz;
                tour.emplace_back();
                for (int i = 0; i < sz; i++) {
                    std::string type, link;
                    std::cin >> type >> link;
                    tour.back().emplace_back(type, link, 1, (int)problems.size() - 1 + i);
                }
                std::cout << "tour created, id = " << tour.size() - 1 << '\n';
            } else {
                std::cout << "invalid option\n";
            }
        } else if (command == "start_tour" && now_tour == -1) {
            int tour_id, time;
            std::cin >> tour_id >> time;
            for (int i = 0; i < tour[tour_id].size(); i++) {
                std::string s = "t/";
                s += char('A' + i);
                s += ".pdf";
                copy_file(tour[tour_id][i].statement_name(), s);
            }
            now_tour = tour_id;
            start_time = clock();
            have_time = time;
        } else if (command == "time" && now_tour != -1) {
            int tm = (clock() - start_time) / CLOCKS_PER_SEC;
            std::cout << tm / 3600 << ':' << (tm % 3600) / 60 << ':' << tm % 60 << '\n';
            tm = have_time - tm;
            std::cout << "remain " << tm / 3600 << ':' << (tm % 3600) / 60 << ':' << tm % 60 << '\n';
        } else {
            std::string com;
            std::getline(std::cin, com);
            com = command + com;
            std::system(com.c_str());
        }
	}
    save_data("save/problems.lox", problems);
    save_data("save/upsolving.lox", upsolving);
    save_data("save/tours.lox", tour);
	write_in_file("temp.sh", "");
	write_in_file("temp.ans", "");
}