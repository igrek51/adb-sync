//
// Created by igrek on 04/02/17.
//

#include "LocalFS.h"
#include "../errors/ParseError.h"
#include "../utils/string_utils.h"
#include "../logger/Logger.h"
#include "../system/CmdExecutor.h"
#include <sstream>
#include <sys/stat.h>

bool LocalFS::pathExists(string path) {
    struct stat info;
    if (stat(path.c_str(), &info) != 0)
        return false; // not exists
    else
        return (info.st_mode & S_IFDIR) != 0;  //is directory ?
}

vector<File*>* LocalFS::listPath(string path) {
    vector<File*>* files = new vector<File*>();

    string output = CommandExecutor::executeAndRead(
            "ls -al --time-style=\"+%Y-%m-%d %H:%M\" \"" + path + "\"");
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

File* LocalFS::parseLsOutput(string lsLine) {
    if (lsLine.empty())
        return nullptr;

    vector<string>* parts = splitByAny(lsLine, "\t ");

    if (parts->size() >= 8) {
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

Directory* LocalFS::parseLsDirectory(vector<string>* parts) {
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

    Directory* dir = new Directory(name);
    return dir;
}

RegularFile* LocalFS::parseLsRegularFile(vector<string>* parts) {
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

    //parsing modification time
    time_t modifiedTime = parseLsTime(modifiedDate + " " + modifiedHour, "%Y-%m-%d %H:%M");

    RegularFile* file = new RegularFile(name);
    file->setSize((unsigned int) stoi(blockSize));
    file->setModifiedDate(modifiedTime);

    return file;
}
