//
// Created by igrek on 04/02/17.
//

#include "ADB.h"
#include "../errors/Error.h"
#include "../logger/Logger.h"
#include "../system/CmdExecutor.h"
#include "../utils/string_utils.h"
#include "../errors/ParseError.h"

#include <sstream>

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
        string output = shell("ls \"" + path + "\"");
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

vector<File*>* ADB::listPath(string path) {
    vector<File*>* files = new vector<File*>();

    string output = shell("ls -al \"" + path + "\"");
    vector<string>* lines = splitLines(output);
    for (string line : *lines) {
        if (endsWith(line, "No such file or directory")) {
            delete lines;
            delete files;
            throw new Error("directory " + path + " does not exist");
        } else if (endsWith(line, "Permission denied")) {
            Logger::warn("listing path " + path + ": " + line);
            continue;
        } else {
            try {
                File* file = parseLsOutput(line);
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

File* ADB::parseLsOutput(string lsLine) {
    if (lsLine.empty())
        return nullptr;

    Logger::debug("parsing ls line: " + lsLine);

    vector<string>* parts = splitByAny(lsLine, "\t ");

    if (parts->size() >= 7) {
        if (parts->at(0).size() == 10) {
            if (startsWith(parts->at(0), "d")) {
                File* result = parseLsDirectory(parts);
                delete parts;
                return result;
            } else if (startsWith(parts->at(0), "-")) {
                File* result = parseLsRegularFile(parts);
                delete parts;
                return result;
            }
        }
    }

    delete parts;
    return nullptr;
}

Directory* ADB::parseLsDirectory(vector<string>* parts) {
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

    // verification
    if (name.length() == 0)
        throw new ParseError("empty directory name");

    Directory* dir = new Directory(name);
    return dir;
}

RegularFile* ADB::parseLsRegularFile(vector<string>* parts) {
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

    //parsing modification time
    time_t modifiedTime = parseLsTime(modifiedDate + " " + modifiedHour, "%Y-%m-%d %H:%M");

    RegularFile* file = new RegularFile(name);
    file->setSize((unsigned int) stoi(blockSize));
    file->setModifiedDate(modifiedTime);

    return file;
}
