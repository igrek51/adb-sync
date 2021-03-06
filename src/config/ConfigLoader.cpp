//
// Created by igrek on 04/02/17.
//

#include "ConfigLoader.h"
#include "../logger/Logger.h"
#include "../utils/string_utils.h"
#include <fstream>
#include <sstream>

const string ConfigLoader::CONFIG_FILENAME = "config.properties";
// TODO move to home/config dir
const string ConfigLoader::DEFAULT_CONFIG_FILENAME = "/mnt/data/Igrek/c++/adb-sync/bin/config.properties";

const string ConfigLoader::CONFIG_DB_PREFIX = "database.";
const string ConfigLoader::CONFIG_LOCAL_PATH_SUFFIX = ".localPath";
const string ConfigLoader::CONFIG_REMOTE_PATH_SUFFIX = ".remotePath";
const string ConfigLoader::CONFIG_EXCLUDED_FILES = "excludedFiles";

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
	if (lines == nullptr) {
		Logger::warn("Failed to load file " + filename);
		return nullptr;
	}

	map<string, string>* variables = new map<string, string>();
	for (string line : *lines) {

		if (line.length() > 0) {
			pair<string, string> variable = parseLine(line);

			if (variable.first.length() != 0) {
				(*variables)[variable.first] = variable.second;
			}
		}
	}
	return new ConfigProperties(variables);
}

ConfigProperties* ConfigLoader::loadProperties() {

	ConfigProperties* properties = loadProperties(CONFIG_FILENAME);
	// if missing load from default config location
	if (properties == nullptr) {
		Logger::warn("missing properties file, loading from default one");
		properties = loadProperties(DEFAULT_CONFIG_FILENAME);
	}

	return properties;
}

pair<string, string> ConfigLoader::parseLine(string line) {
	if (line.length() > 0) {
		if (!startsWith(line, "#")) { // if is not comment

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
	}

	return make_pair("", ""); //empty result
}

vector<Database*>* ConfigLoader::loadDatabases() {

	vector<Database*>* databases = new vector<Database*>();

	ConfigProperties* properties = loadProperties();
	if (properties != nullptr) {

		//TODO search for all keys starting with CONFIG_DB_PREFIX

		// counts databases defined in configuration
		int dbsCount = 0;
		while (properties->keyExists(
				CONFIG_DB_PREFIX + to_string(dbsCount + 1) + CONFIG_LOCAL_PATH_SUFFIX)) {
			dbsCount++;
		}

		for (int dbi = 1; dbi <= dbsCount; dbi++) {
			string localPath = properties->getValue(
					CONFIG_DB_PREFIX + to_string(dbi) + CONFIG_LOCAL_PATH_SUFFIX);
			string remotePath = properties->getValue(
					CONFIG_DB_PREFIX + to_string(dbi) + CONFIG_REMOTE_PATH_SUFFIX);
			Database* db = new Database(localPath, remotePath);
			databases->push_back(db);
		}

	}

	if (databases->empty()) {
		Logger::warn("no database defined in configuration");
	}

	return databases;
}

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	stringstream ss;
	ss.str(s);
	string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

vector<string>* split(std::string &s, char delim) {
	vector<string>* elems = new vector<string>();
	split(s, delim, std::back_inserter(*elems));
	return elems;
}

vector<string>* ConfigLoader::loadExcludedFiles() {

	vector<string>* excludedFiles = new vector<string>();

	ConfigProperties* properties = loadProperties();
	if (properties != nullptr) {

		string excludedFilesA = properties->getValue(CONFIG_EXCLUDED_FILES);
		if(excludedFilesA.length() > 0){
			excludedFiles = split(excludedFilesA, ' ');
		}

	}

	return excludedFiles;
}
