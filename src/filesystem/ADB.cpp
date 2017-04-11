//
// Created by igrek on 04/02/17.
//

#include "ADB.h"
#include "../errors/Error.h"
#include "../logger/Logger.h"
#include "../system/CommandExecutor.h"
#include "../utils/string_utils.h"
#include "../errors/ParseError.h"
#include <sstream>

ADB::ADB() {
	//TODO move to configuration file
	busyboxDirPath = "/data/local/tmp/adb-sync/";
}

void ADB::testADB() {
	try {
		CommandExecutor::executeAndRead("adb version");
	} catch (SystemCmdError* e) {
		Logger::error(e);
		throw new Error("ADB test failed");
	}
}

void ADB::detectDevice() {
	try {
		Logger::info("detecting ADB devices...");
		string output = CommandExecutor::executeAndRead("adb devices");

		vector<string>* lines = splitLines(output);
		for (string line : *lines) {
			if (startsWith(line, "*")) {
				continue;
			}
			if (endsWith(line, "device")) {
				vector<string>* parts = splitByAny(line, "\t ");
				if (parts->size() >= 2) {
					Logger::info("ADB device detected: " + parts->at(0));
					delete parts;
					delete lines;
					return;
				}
				delete parts;
			}
		}

		delete lines;

	} catch (SystemCmdError* e) {
		Logger::error(e);
		throw new Error("ADB detecting device failed");
	}
	throw new Error("ADB device not found");
}

void ADB::checkBusyBox() {
	// check if directory for busybox exists in tmp folder
	if (!pathExists(busyboxDirPath)) {
		Logger::debug("creating directory for busybox: " + busyboxDirPath);
		shell("mkdir " + busyboxDirPath);
	}
	// check if busybox exists
	string busyboxPath = busyboxDirPath + "busybox";
	if (!pathExists(busyboxPath)) {
		Logger::debug("copying busybox...");
		push("busybox", busyboxPath);
	}
	// check if symbolic links exists
	string cksumPath = busyboxDirPath + "cksum";
	if (!pathExists(cksumPath)) {
		Logger::debug("creating cksum link...");
		shell("ln -s " + busyboxPath + " " + cksumPath);
	}
}

string ADB::shell(string cmd) {
	try {
		return CommandExecutor::executeAndRead("adb shell " + cmd);
	} catch (SystemCmdError* e) {
		Logger::error(e);
		throw new Error("ADB shell failed: " + cmd);
	}
}

bool ADB::pathExists(string path) {
	try {
		string output = shell("ls " + escapeShellPath(path));
		vector<string>* lines = splitLines(output);

		// check if there is a line with nonexisting directory message
		if (lines->size() <= 3) {
			for (string line : *lines) {
				if (endsWith(line, "No such file or directory")) {
					delete lines;
					return false;
				}
			}
		}

		delete lines;
		return true;
	} catch (Error* e) {
		// exit code != 0
		Logger::error(e);
		return false;
	}
}

RegularFile* ADB::getRegularFileDetails(string path, string name) {
	RegularFile* file = new RegularFile(path, name);

	// get output from cksum: CRC checksum, total size (bytes), filename
	//TODO do not check crc checksum if file is big (exceeds some filesize threshold), then check only total size
	string output = shell(busyboxDirPath +
						  "cksum " + escapeShellPath(file->getFullPathName()));
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


vector<File*>* ADB::listPath(string path) {
	vector<File*>* files = new vector<File*>();

	//TODO use platform independent binaries
	string output = shell(
			"ls -al " + escapeShellPath(path));
	vector<string>* lines = splitLines(output);
	for (string line : *lines) {
		if (endsWith(line, "No such file or directory")) {
			delete lines;
			delete files;
			throw new Error("remote directory " + path + " does not exist");
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

File* ADB::parseLsOutput(string path, string lsLine) {
	if (lsLine.empty())
		return nullptr;

//    Logger::debug("parsing ls line: " + lsLine);
	vector<string>* parts = splitByAny(lsLine, "\t ");

	if (parts->size() >= 7) {
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

Directory* ADB::parseLsDirectory(string path, vector<string>* parts) {
	unsigned int index = 0;
	string permissions = nextNonemptyPart(parts, index);
	string owner = nextNonemptyPart(parts, index);
	string group = nextNonemptyPart(parts, index);
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

RegularFile* ADB::parseLsRegularFile(string path, vector<string>* parts) {
	unsigned int index = 0;
	string permissions = nextNonemptyPart(parts, index);
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


void ADB::push(string localPath, string remotePath) {
	CommandExecutor::execute("adb push " + escapePath(localPath) + " " + escapePath(remotePath));
}

void ADB::pull(string localPath, string remotePath) {
	CommandExecutor::execute("adb pull " + escapePath(remotePath) + " " + escapePath(localPath));
}

void ADB::removeFile(string path) {
	shell("rm " + escapeShellPath(path));
}

void ADB::removeDirectory(string path) {
	shell("rm -rf " + escapeShellPath(path));
}

string ADB::escapeShellPath(string path) {
	string result = "\\\"" + path + "\\\"";
	// escaping single characters
	result = replaceAll(result, "'", "\\'");
	result = replaceAll(result, "&", "\\&");
	result = replaceAll(result, "(", "\\(");
	result = replaceAll(result, ")", "\\)");
	return result;
}

string ADB::escapePath(string path) {
	// 1. escaping quote as \" in system command
	// 2. escaping backslash as \\ and " as \" in cpp file
	return "\"" + path + "\"";
}

