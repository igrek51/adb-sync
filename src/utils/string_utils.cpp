//
// Created by igrek on 05/02/17.
//

#include "string_utils.h"
#include <boost/algorithm/string.hpp>

string trimSpaces(string s) {
	//delete spaces from the end
	while (s.length() > 0 && s[s.length() - 1] == ' ') {
		s = s.substr(0, s.length() - 1);
	}
	//delete spaces from the beginning
	while (s.length() > 0 && s[0] == ' ') {
		s = s.substr(1);
	}
	return s;
}

bool startsWith(string s, string prefix) {
	if (prefix.length() == 0) return true;
	if (prefix.length() > s.length()) return false;
	s = s.substr(0, prefix.length());
	return s == prefix;
}

bool endsWith(string s, string suffix) {
	if (suffix.length() == 0) return true;
	if (suffix.length() > s.length()) return false;
	s = s.substr(s.length() - suffix.length(), suffix.length());
	return s == suffix;
}

vector<string>* splitLines(string s) {
	vector<string>* strs = new vector<string>();
	boost::split(*strs, s, boost::is_any_of("\r\n"));
	return strs;
}

vector<string>* splitByAny(string s, string delimiters) {
	vector<string>* strs = new vector<string>();
	boost::split(*strs, s, boost::is_any_of(delimiters));
	return strs;
}

string replaceAll(string str, const string& from, const string& to) {
	string result = str;
	if (from.empty())
		return result;
	size_t start_pos = 0;
	while ((start_pos = result.find(from, start_pos)) != std::string::npos) {
		result.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
	return result;
}


string time2string(time_t t, string pattern) {
	char buff[20];
	strftime(buff, 20, pattern.c_str(), localtime(&t));
	return string(buff);
}

string to_string(const char* s) {
	return string(s);
}

string nextNonemptyPart(vector<string>* parts, unsigned int& index) {
	for (; index < parts->size(); index++) {
		string& part = parts->at(index);
		if (!part.empty()) {
			index++;
			return part;
		}
	}
	return "";
}