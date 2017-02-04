#include "path.h"
#include "string_utils.h"

Path::Path(string path) {
    this->path = cutSlashFromEnd(reformatSlash(path));
}

string Path::getPath() {
    return getUnixPath();
}

string Path::getUnixPath() {
    return path;
}

string Path::getWindowsPath() {
    return replaceStr(path, '/', '\\');
}

string Path::cutSlashFromBeginning(string pathstr) {
    while (pathstr.length() > 0 && pathstr.at(0) == '/') {
        pathstr = pathstr.substr(1);
    }
    return pathstr;
}

string Path::cutSlashFromEnd(string pathstr) {
    while (pathstr.length() > 0 && pathstr.at(pathstr.length() - 1) == '/') {
        pathstr = pathstr.substr(0, pathstr.length() - 1);
    }
    return pathstr;
}

string Path::reformatSlash(string pathstr) {
    return replaceStr(pathstr, '\\', '/');
}

bool Path::endsWith(string ext) {
    if (ext.length() == 0) return true;
    if (ext.length() > path.length()) return false;
    string cmp = path.substr(path.length() - ext.length(), ext.length());
    return cmp.compare(ext) == 0;
}

Path *Path::removeExtension() {
    size_t lastdot = path.rfind('.');
    if (lastdot != string::npos && lastdot > 0) {
        size_t lastslash = path.rfind('/');
        if (lastslash != string::npos &&
            lastslash > lastdot) { //jeśli kropka jest przed ostatnim slashem
            return this;
        }
        //usunięcie rozszerzenia
        path = path.substr(0, lastdot);
    }
    return this;
}

Path *Path::append(string child) {
    Path *ch = new Path(child);
    Path *result = this->append(ch);
    delete ch;
    return result;
}

Path *Path::append(Path *child) {
    string parent = cutSlashFromEnd(this->path);
    if (parent.length() == 0) {
        this->path = cutSlashFromBeginning(child->path);
    } else {
        this->path = cutSlashFromEnd(this->path) + "/" + cutSlashFromBeginning(child->path);
    }
    return this;
}

string Path::reformat(string path) {
    Path *p = new Path(path);
    string result = p->getPath();
    delete p;
    return result;
}

string Path::append(string dir, string filename) {
    Path *parent = new Path(dir);
    string result = parent->append(filename)->getPath();
    delete parent;
    return result;
}

string Path::removeExtenstion(string filename) {
    Path *ch = new Path(filename);
    string result = ch->removeExtension()->getPath();
    delete ch;
    return result;
}

string Path::formatUnderscore(string filename){
    Path *ch = new Path(filename);
    string result = replaceStr(ch->removeExtension()->path, '/', '_');
    delete ch;
    return result;
}

Path* Path::parentDir(){
    size_t lastslash = path.rfind('/');
    if(lastslash == string::npos){
        path = "";
    }else {
        path = path.substr(0, lastslash);
    }
    return this;
}