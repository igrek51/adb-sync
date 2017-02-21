//
// Created by igrek on 05/02/17.
//

#include "ConfigProperties.h"

ConfigProperties::ConfigProperties(map<string, string>* variables) {
	this->variables = variables;
}

ConfigProperties::~ConfigProperties() {
	variables->clear();
}

bool ConfigProperties::keyExists(string key) {
	auto it = variables->find(key);
	return it != variables->end();
}

string ConfigProperties::getValue(string key, string defaultValue) {
	auto it = variables->find(key);
	if (it == variables->end()) {
		return defaultValue;
	} else {
		return it->second;
	}
}

vector<string>* ConfigProperties::getKeys() {
	vector<string>* keys = new vector<string>();
	for (auto it = variables->begin(); it != variables->end(); ++it) {
		keys->push_back(it->first);
	}
	return keys;
}
