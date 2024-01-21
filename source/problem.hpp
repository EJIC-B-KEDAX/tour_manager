#ifndef PROBLEM_H_
#define PROBLEM_H_ 1

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "url_data.hpp"
#include "useful_functions.hpp"

class Problem {
public:
	Problem(std::string name, std::string type, int complexity, int index, std::vector<int> submissions = std::vector<int>()) {
		_name = name;
		_type = type;
		_complexity = complexity;
		_index = index;
		_submissions = submissions;
	}
	Problem() = default;
	void submit(std::string code) {
		code = string_to_url(code);
		std::string request = oj_to_problem;
		replace(request, "{SESSION}", oj_session);
		replace(request, "{NAME}", _name);
		write_in_file("temp.sh", request);
		std::system("temp.sh");
		if (!oj_update_data()) {
			return;
		}
		request = oj_submit;
		replace(request, "{SESSION}", oj_session);
		replace(request, "{NAME}", _name);
		replace(request, "{CODE}", code);
		replace(request, "{CSRF_TOKEN}", oj_csrf_token);
		int pos = (int)request.find("--data-binary $\'") + 16, len = 0;
		while (request[pos] != '\'') {
			pos++;
			len++;
		}
		replace(request, "{LENGTH}", std::to_string(len));
		write_in_file("temp.sh", request);
		std::system("temp.sh");
		if (!oj_update_data()) {
			return;
		}
		std::string ans = read_from_file("temp.ans");
		if (ans.find("submission/") >= ans.size()) {
			std::cout << "something went wrong\n";
			return;
		}
		std::string submission_id;
		pos = (int)ans.find("submission/") + 11;
		while (ans[pos] != '\n') {
			submission_id += ans[pos];
			pos++;
		}
		_submissions.push_back(stoi(submission_id));
		std::cout << "submission sent\n";
	}
	std::ofstream& operator << (std::ofstream& fout) {
		fout << _name << " " << _type << " " << _complexity << " " << _index << "\n" << _submissions.size() << "\n";
		for (int i : _submissions) {
			fout << (i) << " ";
		}
		fout << std::to_string('\n');
		return fout;
	}
private:
	std::string _name, _type;
	int _complexity, _index;
	std::vector<int> _submissions;
};

#endif