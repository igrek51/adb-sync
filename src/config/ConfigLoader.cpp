//
// Created by igrek on 04/02/17.
//

#include "ConfigLoader.h"
#include "../logger/Logger.h"
#include <fstream>
#include <sstream>

const string ConfigLoader::CONFIG_FILENAME = "config.properties";

const string ConfigLoader::CONFIG_DB_PREFIX = "database.";
const string ConfigLoader::CONFIG_LOCAL_PATH_SUFFIX = ".localPath";
const string ConfigLoader::CONFIG_REMOTE_PATH_SUFFIX = ".remotePath";

ConfigLoader::ConfigLoader() {

}

ConfigLoader::~ConfigLoader() {

}

vector<string>* ConfigLoader::loadFileLines(string filename) {
    fstream plik;
    plik.open(filename.c_str(), fstream::in | fstream::binary);
    if (!plik.good()) {
        plik.close();
        return nullptr;
    }
    vector<string>* lines = new vector<string>;
    string line;
    do {
        getline(plik, line, '\n'); //split by LF
        for (unsigned int i = 0; i < line.length(); i++) { //remove CR characters
            if (line[i] == '\r') {
                line.erase(line.begin() + i);
                i--;
            }
        }
        lines->push_back(line);
    } while (!plik.eof());
    plik.close();
    return lines;
}

ConfigProperties* ConfigLoader::loadProperties(string filename) {
    vector<string>* lines = loadFileLines(filename);
    if(lines == nullptr){
        Logger::warn("Failed to load file " + filename);
        return nullptr;
    }

    map<string, string>* variables = new map<string, string>();
    for(string line : *lines){
        if(line.length() > 0){
            pair<string, string> variable = parseLine(line);
            if(variable.first.length() != 0){
                variables->at(variable.first) = variable.second;
            }
        }
    }
    return new ConfigProperties(variables);
}

pair<string, string> ConfigLoader::parseLine(string line) {
    if (line.length() > 0) {

        for (unsigned int j = 1; j < line.length(); j++) {
            if (line.at(j) == '=') {
                string key = line.substr(0, j);
                string value = line.substr(j + 1);
                key = trimSpaces(key);
                value = trimSpaces(value);
                return make_pair(key, value);
            }
        }

    }

    return make_pair("", "");
}

string ConfigLoader::trimSpaces(string s) {
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

string ConfigLoader::itos(int number) {
    stringstream ss;
    ss << number;
    return ss.str();
}

bool ConfigLoader::beginsWith(string s, string prefix){
    if (prefix.length() == 0) return true;
    if (prefix.length() > s.length()) return false;
    s = s.substr(0, prefix.length());
    return s == prefix;
}

bool ConfigLoader::containsKeyBeginningWith(vector<string>* keys, string prefixKey) {
    for(string key : *keys){
        if(beginsWith(key, prefixKey)){
            return true;
        }
    }
    return false;
}

vector<Database*>* ConfigLoader::loadDatabases() {

    ConfigProperties* properties = loadProperties(CONFIG_FILENAME);

    vector<Database*>* databases = new vector<Database*>();

    vector<string>* keys = properties->getKeys();

    // counts databases defined in configuration
    int dbsCount = 0;
    while(properties->keyExists(CONFIG_DB_PREFIX + itos(dbsCount+1) + CONFIG_LOCAL_PATH_SUFFIX)){
        dbsCount++;
    }

    for(int dbi = 1; dbi <= dbsCount; dbi++){
        string localPath = properties->getValue(CONFIG_DB_PREFIX + itos(dbi) + CONFIG_LOCAL_PATH_SUFFIX);
        string remotePath = properties->getValue(CONFIG_DB_PREFIX + itos(dbi) + CONFIG_REMOTE_PATH_SUFFIX);
        Database* db = new Database(localPath, remotePath);
        databases->push_back(db);
    }

    if(databases->empty()){
        Logger::warn("no database defined in configuration");
    }

    return databases;
}


