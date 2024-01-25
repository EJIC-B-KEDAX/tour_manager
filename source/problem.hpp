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
        run_file("temp.sh");
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
        if (ans.find(" ms") >= ans.size()) {
            std::cout << "something went wrong\n";
            return;
        }
        std::string time_limit, memory_limit;
        pos = (int)ans.find(" ms") - 1;
        while (ans[pos] != '>') {
            time_limit += ans[pos];
            pos--;
        }
        reverse(time_limit.begin(), time_limit.end());
        if (ans.find(" MiB") >= ans.size()) {
            std::cout << "something went wrong\n";
            return;
        }
        pos = (int)ans.find(" MiB") - 1;
        while (ans[pos] != '>') {
            memory_limit += ans[pos];
            pos--;
        }
        reverse(memory_limit.begin(), memory_limit.end());
        _time_limit = stoi(time_limit);
        _memory_limit = stoi(memory_limit);
        download_file(link_to_statement, statement_name());
    }
	void submit(std::string code) {
        code = string_to_url(code);
        std::string request = oj_to_problem;
        replace(request, "{SESSION}", oj_session);
        replace(request, "{NAME}", _name);
        write_in_file("temp.sh", request);
        run_file("temp.sh");
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
        run_file("temp.sh");
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
    bool submission_info(int submission_id) {
        submission_id = _submissions[submission_id];
        std::string request = oj_to_submission;
        replace(request, "{SUBMISSION}", std::to_string(submission_id));
        write_in_file("temp.sh", request);
        run_file("temp.sh");
        std::string ans = read_from_file("temp.ans");
        std::vector<std::string> subtask;
        if (ans.find("progressbar_text_" + std::to_string(submission_id)) >= ans.size()) {
            std::cout << "something went wrong\n";
            return true;
        }
        int pos = (int)ans.find("progressbar_text_" + std::to_string(submission_id)) + 56;
        std::string verdict;
        while (ans[pos] != '<') {
            verdict += ans[pos];
            pos++;
        }
        if (verdict == "Compilation error") {
            std::cout << verdict << '\n';
            return true;
        }
        if (verdict[0] < '0' || verdict[0] > '9') {
            std::cout << "testing\n";
            return false;
        }
        std::string score, time, memory;
        pos = (int)ans.find("progressbar_text_" + std::to_string(submission_id)) + (int)("progressbar_text_" + std::to_string(submission_id)).size() + 33;
        while (ans[pos] != ' ') {
            score += ans[pos];
            pos++;
        }
        pos = (int)ans.find("max_execution_time_" + std::to_string(submission_id)) + (int)("max_execution_time_" + std::to_string(submission_id)).size() + 2;
        while (ans[pos] != '<') {
            time += ans[pos];
            pos++;
        }
        pos = (int)ans.find("max_memory_" + std::to_string(submission_id)) + (int)("max_memory_" + std::to_string(submission_id)).size() + 2;
        while (ans[pos] != '<') {
            memory += ans[pos];
            pos++;
        }
        while (ans.find(".0</div>") < ans.size()) {
            pos = (int)ans.find(".0</div>") - 1;
            subtask.emplace_back();
            while (ans[pos] != '>') {
                subtask.back() += ans[pos];
                pos--;
            }
            reverse(subtask.back().begin(), subtask.back().end());
            subtask.back() += ".0";
            ans.replace(ans.find(".0</div>"), 8, "used");
        }
        std::cout << "score: " << score << '\n' << "time: " << time << '\n' << "memory: " << memory << '\n' << "subtasks: " << '\n';
        for (const auto& i : subtask) {
            std::cout << i << '\n';
        }
        _score = std::max(_score, stoi(score));
        return true;
    }
    int get_score() {
        return _score;
    }
    int number_of_submissions() {
        return (int)_submissions.size();
    }
    int get_complexity() {
        return _complexity;
    }
    int get_time_limit() {
        return _time_limit;
    }
    int get_memory_limit() {
        return _memory_limit;
    }
    friend std::ifstream& operator >> (std::ifstream& fin, Problem& a);
	friend std::ofstream& operator << (std::ofstream& fout, Problem& a);
private:
	std::string _name, _type;
	int _complexity{}, _index{}, _score{}, _time_limit{}, _memory_limit{};
	std::vector<int> _submissions;
};

std::ifstream& operator >> (std::ifstream& fin, Problem& a) {
    fin >> a._name >>  a._type >> a._complexity >> a._index >> a._score >> a._time_limit >> a._memory_limit;
    int sz;
    fin >> sz;
    a._submissions.resize(sz);
    for (int i = 0; i < sz; i++) {
        fin >> a._submissions[i];
    }
    return fin;
}

std::ofstream& operator << (std::ofstream& fout, Problem& a) {
    fout << a._name << " " << a._type << " " << a._complexity << " " << a._index << ' ' << a._score << ' ' << a._time_limit << ' ' << a._memory_limit << "\n" << a._submissions.size() << "\n";
    for (int i: a._submissions) {
        fout << i << " ";
    }
    fout << '\n';
    return fout;
}

#endif