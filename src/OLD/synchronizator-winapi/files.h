#ifndef FILES_H
#define FILES_H

#include <iostream>
#include <vector>

using namespace std;

class File {
public:
    File(string name, int size = -1);
	string name;
	int size;
};

void clear_file(string filename);
bool file_exists(string name);
bool dir_exists(string name);
bool files_cmp(string file1, string file2, bool out = false);

vector<string>* get_all_lines(string filename);
vector<string>* get_nonempty_lines(string filename);

char* open_file(string filename, int &file_size);
bool save_file(string filename, string content);

File* file_search(vector<File*>* files, string name);
vector<File*>* list_dir(string dir);
vector<string>* get_drives();

#endif
