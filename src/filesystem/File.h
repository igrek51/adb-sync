//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_FILE_H
#define ADBSYNC_FILE_H

#include <string>

using namespace std;

class File {
public:
	File(string path, string name);

	virtual ~File();

	string getName();

	void setName(const string& name);

	string getPath();

	void setPath(const string& path);

	string getFullPathName();

	static string subfolder(string path, string dirName);

protected:
	string name;
	string path;
};


#endif //ADBSYNC_FILE_H
