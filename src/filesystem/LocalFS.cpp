//
// Created by igrek on 04/02/17.
//

#include "LocalFS.h"
#include "../errors/ParseError.h"
#include "../utils/string_utils.h"
#include "../logger/Logger.h"
#include "../system/CommandExecutor.h"
#include <sys/stat.h>
#include <sstream>

bool LocalFS::pathExists(string path) {
	struct stat info;
	if (stat(path.c_str(), &info) != 0)
		return false; // not exists
	else
		return (info.st_mode & S_IFDIR) != 0;  //is directory ?
}

RegularFile* LocalFS::getRegularFileDetails(string path, string name) {
	RegularFile* file = new RegularFile(path, name);

	// get output from cksum: CRC checksum, total size (bytes), filename
	string output = CommandExecutor::executeAndRead(
			"cksum " + escapePath(file->getFullPathName()));
	vector<string>* parts = splitByAny(output, " \n\r");
	if (parts->size() < 3) {
		delete parts;
		throw new ParseError("not enough parts to parse: " + parts->size());
	}
	unsigned int index = 0;
	string crcChecksum = nextNonemptyPart(parts, index);
	string sizeStr = nextNonemptyPart(parts, index);
	delete parts;

	// validation
	if (crcChecksum.empty())
		throw new ParseError("CRC checksum part not found: " + output);
	if (sizeStr.empty())
		throw new ParseError("total size part not found: " + output);

	try {
		file->setSize((unsigned int) stoi(sizeStr));
	} catch (std::invalid_argument e) {
		throw new ParseError("invalid total file size: " + sizeStr);
	}

	file->setChecksum(crcChecksum);

	return file;
}


vector<File*>* LocalFS::listPath(string path) {
	vector<File*>* files = new vector<File*>();

	string output = CommandExecutor::executeAndRead(
			"ls -al --time-style=\"+%Y-%m-%d %H:%M\" " + escapePath(path));
	vector<string>* lines = splitLines(output);
	for (string line : *lines) {
		if (endsWith(line, "No such file or directory")) {
			delete lines;
			delete files;
			throw new Error("local directory " + path + " does not exist");
		} else if (endsWith(line, "Permission denied")) {
			Logger::warn("listing path " + path + ": " + line);
			continue;
		} else if (startsWith(line, "total ")) {
			continue; // skipping line with total elements
		} else {
			try {
				File* file = parseLsOutput(path, line);
				if (file != nullptr) {
					files->push_back(file);
				}
			} catch (ParseError* e) {
				Logger::error(e);
			}
		}
	}

	delete lines;
	return files;
}

File* LocalFS::parseLsOutput(string path, string lsLine) {
	if (lsLine.empty())
		return nullptr;

//    Logger::debug("parsing ls line: " + lsLine);
	vector<string>* parts = splitByAny(lsLine, "\t ");

	if (parts->size() >= 8) {
		if (parts->at(0).size() == 10) {
			if (startsWith(parts->at(0), "d")) {
				File* result = parseLsDirectory(path, parts);
				delete parts;
				return result;
			} else if (startsWith(parts->at(0), "-")) {
				File* result = parseLsRegularFile(path, parts);
				delete parts;
				return result;
			}
		}
	}

	delete parts;
	return nullptr;
}

Directory* LocalFS::parseLsDirectory(string path, vector<string>* parts) {
	unsigned int index = 0;
	string permissions = nextNonemptyPart(parts, index);
	string num1 = nextNonemptyPart(parts, index);
	string owner = nextNonemptyPart(parts, index);
	string group = nextNonemptyPart(parts, index);
	string blockSize = nextNonemptyPart(parts, index);
	string modifiedDate = nextNonemptyPart(parts, index);
	string modifiedHour = nextNonemptyPart(parts, index);
	stringstream ss;
	for (; index < parts->size(); index++) {
		string& part = parts->at(index);
		ss << part;
		if (index < parts->size() - 1) { // not last part
			ss << " ";
		}
	}
	string name = ss.str();

	if (name == "." || name == "..") {
		return nullptr; // skip symbolic folders
	}

	// verification
	if (name.length() == 0)
		throw new ParseError("empty directory name");

	return new Directory(path, name);
}

RegularFile* LocalFS::parseLsRegularFile(string path, vector<string>* parts) {
	unsigned int index = 0;
	string permissions = nextNonemptyPart(parts, index);
	string num1 = nextNonemptyPart(parts, index);
	string owner = nextNonemptyPart(parts, index);
	string group = nextNonemptyPart(parts, index);
	string blockSize = nextNonemptyPart(parts, index);
	string modifiedDate = nextNonemptyPart(parts, index);
	string modifiedHour = nextNonemptyPart(parts, index);
	stringstream ss;
	for (; index < parts->size(); index++) {
		string& part = parts->at(index);
		ss << part;
		if (index < parts->size() - 1) { // not last part
			ss << " ";
		}
	}
	string name = ss.str();

	if (name.length() == 0)
		throw new ParseError("empty filename");
	if (blockSize.length() == 0)
		throw new ParseError("empty blockSize");
	if (modifiedDate.length() == 0)
		throw new ParseError("empty modifiedDate");
	if (modifiedHour.length() == 0)
		throw new ParseError("empty modifiedHour");

	return getRegularFileDetails(path, name);
}


string LocalFS::escapePath(string path) {
	// adding quotes
	// 1. escaping quote as \" in system command
	// 2. escaping backslash as \\ and " as \" in cpp file
	return "\"" + path + "\"";
}

void LocalFS::removeFile(string path) {
	CommandExecutor::execute("rm " + escapePath(path));
}

void LocalFS::removeDirectory(string path) {
	CommandExecutor::execute("rm -rf " + escapePath(path));
}
