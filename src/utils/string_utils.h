//
// Created by igrek on 05/02/17.
//

#ifndef ADBSYNC_STRING_UTILS_H
#define ADBSYNC_STRING_UTILS_H

#include <string>
#include <vector>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

string trimSpaces(string s);

bool startsWith(string s, string prefix);

bool endsWith(string s, string suffix);

vector<string>* splitLines(string s);

vector<string>* splitByAny(string s, string delimiters);

/**
 * @param time boost::posix_time::ptime
 * @param pattern time pattern, example: %Y-%m-%d %H:%M:%S
 * @return boost::posix_time::ptime converted to string
 */
string time2string(boost::posix_time::ptime time, string pattern);

/**
 * @param s
 * @param pattern time pattern, example: %Y-%m-%d %H:%M:%S
 * @return string converted to boost::posix_time::ptime or boost::posix_time::ptime() if failed
 */
boost::posix_time::ptime string2time(string s, string pattern);

string replaceAll(string str, const string& from, const string& to);

#endif //ADBSYNC_STRING_UTILS_H
