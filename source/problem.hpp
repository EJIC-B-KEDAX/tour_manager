#ifndef PROBLEM_H_
#define PROBLEM_H_ 1

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "work_with_url.hpp"
#include "useful_functions.hpp"

class Problem {
public:
	Problem(std::string name, std::string type, int complexity, int index, std::vector<int> submissions) {
		_name = name;
		_type = type;
		_complexity = complexity;
		_index = index;
		_submissions = submissions;
	}
	Problem() = default;
    Problem(std::string type, std::string link, int complexity, int index) {
        _type = type;
        _name.clear();
        _complexity = complexity;
        _index = index;
        _submissions = std::vector<int>();
        int ind = (int)link.size() - 1;
        while (link[ind] != '/') {
            _name = link[ind] + _name;
            ind--;
        }
        std::string request, ans;
        request = oj_view_problem;
        replace(request, "{NAME}", _name);
        write_in_file("temp.sh", request);
        std::system("temp.sh");
        ans = read_from_file("temp.ans");
        if (ans.find("Your browser does not support embedded PDF files. Please download the <a href='") >= ans.size()) {
            std::cout << "something went wrong\n";
            return;
        }
        int pos = (int)ans.find("Your browser does not support embedded PDF files. Please download the <a href='") + 79;
        std::string link_to_statement;
        while (ans[pos] != '\'') {
            link_to_statement += ans[pos];
            pos++;
        }
        download_file(link_to_statement, statement_name());
    }
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
        int pos = (int) request.find("--data-binary $\'") + 16, len = 0;
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
        pos = (int) ans.find("submission/") + 11;
        while (ans[pos] != '\n') {
            submission_id += ans[pos];
            pos++;
        }
        _submissions.push_back(stoi(submission_id));
        std::cout << "submission sent\n";
    }
    std::string statement_name() const {
        return "save/" + std::to_string(_complexity) + "_" + std::to_string(_index) + ".pdf";
    }
    std::string get_name() {
        return _name;
    }
    friend std::ifstream& operator >> (std::ifstream& fin, Problem& a);
	friend std::ofstream& operator << (std::ofstream& fout, Problem& a);
private:
	std::string _name, _type;
	int _complexity{}, _index{};
	std::vector<int> _submissions;
};

std::ifstream& operator >> (std::ifstream& fin, Problem& a) {
    fin >> a._name >>  a._type >> a._complexity >> a._index;
    int sz;
    fin >> sz;
    a._submissions.resize(sz);
    for (int i = 0; i < sz; i++) {
        fin >> a._submissions[i];
    }
    return fin;
}

std::ofstream& operator << (std::ofstream& fout, Problem& a) {
    fout << a._name << " " << a._type << " " << a._complexity << " " << a._index << "\n" << a._submissions.size() << "\n";
    for (int i: a._submissions) {
        fout << i << " ";
    }
    fout << '\n';
    return fout;
}

#endif