//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_CONFIGLOADER_H
#define ADBSYNC_CONFIGLOADER_H

#include <iostream>
#include <map>
#include <vector>
#include "ConfigProperties.h"
#include "Database.h"

using namespace std;

class ConfigLoader {
public:
	ConfigLoader();

	~ConfigLoader();

	vector<Database*>* loadDatabases();

	vector<string>* loadExcludedFiles();

private:
	ConfigProperties* loadProperties();

	ConfigProperties* loadProperties(string filename);

	vector<string>* loadFileLines(string filename);

	pair<string, string> parseLine(string line);

	static const string CONFIG_FILENAME;
	static const string DEFAULT_CONFIG_FILENAME;

	static const string CONFIG_DB_PREFIX;
	static const string CONFIG_LOCAL_PATH_SUFFIX;
	static const string CONFIG_REMOTE_PATH_SUFFIX;
	static const string CONFIG_EXCLUDED_FILES;
};


#endif //ADBSYNC_CONFIGLOADER_H
