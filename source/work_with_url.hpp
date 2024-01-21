#ifndef URL_DATA_
#define URL_DATA_ 1

#include <string>
#include <fstream>
#include <iostream>
#include "useful_functions.hpp"

std::string oj_session;

std::string oj_csrf_token;

std::string oj_start = "curl -i -s -k -X $'GET' $'https://oj.uz/login' > temp.ans";

std::string oj_login = "curl -i -s -k -X $'POST' -H $'Host: oj.uz' -H $'Content-Length: {LENGTH}' -H $'Content-Type: application/x-www-form-urlencoded' -H $'Referer: https://oj.uz/login' -b $'session={SESSION}' --data-binary $'next=&csrf_token={CSRF_TOKEN}&email={EMAIL}&password={PASSWORD}&next=&submit=Sign+in' $'https://oj.uz/login?next=' > temp.ans";

std::string oj_to_problem = "curl -i -s -k -X $'GET' -H $'Host: oj.uz' -b $'session={SESSION}' $'https://oj.uz/problem/submit/{NAME}' > temp.ans";

std::string oj_submit = "curl -i -s -k -X $'POST' -H $'Host: oj.uz' -H $'Content-Length: {LENGTH}' -H $'Content-Type: application/x-www-form-urlencoded' -H $'Referer: https://oj.uz/problem/submit/{NAME}' -b $'session={SESSION}' --data-binary $'codes=&csrf_token={CSRF_TOKEN}&language=9&code_1={CODE}&codes=' $'https://oj.uz/problem/submit/{NAME}' > temp.ans";

std::string oj_to_sibmission = "curl -i -s -k -X $'GET' -H $'Host: oj.uz' -H $'Referer: https://oj.uz/problem/submit/{NAME}' -b $'session={SESSION}' $'https://oj.uz/submission/{SUBMISSION}' > temp.ans";

std::string oj_view_problem = "curl -i -s -k -X $'GET' https://oj.uz/problem/view/{NAME} > temp.ans";

bool oj_update_data() {
	std::string ans = read_from_file("temp.ans");
	if (ans.find("302 Found") >= ans.size() && ans.find("200 OK") >= ans.size()) {
		std::cout << "something went wrong\n";
		return false;
	}
	if (ans.find("session=") < ans.size()) {
		int pos = (int)ans.find("session=") + 8;
		oj_session.clear();
		while (ans[pos] != ';') {
            oj_session += ans[pos];
			pos++;
		}
	}
	if (ans.find(R"(name="csrf_token" type="hidden" value=")") < ans.size()) {
		int pos = (int)ans.find(R"(name="csrf_token" type="hidden" value=")") + 39;
		oj_csrf_token.clear();
		while (ans[pos] != '\"') {
			oj_csrf_token += ans[pos];
			pos++;
		}
	}
	return true;
}

bool oj_sign_in() {
	std::string email, password, request;
	std::cout << "Email or handle: ";
	std::cin >> email;
	std::cout << "Password: ";
	std::cin >> password;
	email = string_to_url(email);
	password = string_to_url(password);
	write_in_file("temp.sh", oj_start);
	std::system("temp.sh");
	if (!oj_update_data()) {
		std::cout << "sign in failed\n";
		return false;
	}
	request = oj_login;
	replace(request, "{SESSION}", oj_session);
	replace(request, "{EMAIL}", email);
	replace(request, "{PASSWORD}", password);
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
		std::cout << "sign in failed\n";
		return false;
	}
	std::string ans = read_from_file("temp.ans");
	if (ans.find("302 Found") >= ans.size()) {
		std::cout << "Email or password incorrect\n";
		return false;
	}
	std::cout << "authorization successful\n";
	return true;
}

void download_file(std::string link, std::string filename) {
    write_in_file("temp.sh", "curl " + link + " > " + filename);
    std::system("temp.sh");
}

#endif