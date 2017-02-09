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


string time2string(boost::posix_time::ptime time, string pattern) {
    boost::posix_time::time_facet* facet = new boost::posix_time::time_facet();
    facet->format(pattern.c_str());
    stringstream stream;
    stream.imbue(std::locale(std::locale::classic(), facet));
    stream << time;
    return stream.str();
}

boost::posix_time::ptime string2time(string s, string pattern) {
    const std::locale format = std::locale(std::locale::classic(),
                                           new boost::posix_time::time_input_facet(pattern));
    boost::posix_time::ptime pt;
    std::istringstream is(s);
    is.imbue(format);
    is >> pt;
    return pt;
}
