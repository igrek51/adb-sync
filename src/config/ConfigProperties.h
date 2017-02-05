//
// Created by igrek on 05/02/17.
//

#ifndef ADBSYNC_CONFIGPROPERTIES_H
#define ADBSYNC_CONFIGPROPERTIES_H

#include <map>
#include <string>
#include <vector>

using namespace std;

class ConfigProperties {
public:
    ConfigProperties(map<string, string>* variables);

    ~ConfigProperties();

    bool keyExists(string key);

    string getValue(string key, string defaultValue = "");

    vector<string>* getKeys();

private:
    map<string, string>* variables;
};


#endif //ADBSYNC_CONFIGPROPERTIES_H
