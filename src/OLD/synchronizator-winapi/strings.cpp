#include "strings.h"
#include <ctime>

string trim_spaces(string s){
    //obciêcie spacji na koñcu
    while(s.length() > 0  && s[s.length()-1] == ' '){
        s = s.substr(0, s.length()-1);
    }
    //obciêcie spacji na pocz¹tku
    while(s.length() > 0  && s[0] == ' '){
        s = s.substr(1);
    }
    return s;
}

string trim_quotes(string s){
    if(s.length()>=3){
        //jeœli cudzys³owy s¹ na poczatku i na koñcu
        if(s[0]=='\"' && s[s.length()-1]=='\"'){
            //jeœli w ca³ym stringu znajduj¹ siê tylko 2 cudzys³owy
            if(s.substr(1, s.length()-2).find('\"')==string::npos){
                s = s.substr(1, s.length()-2); //usuñ je
            }
        }
    }
    return s;
}

char to_lowercase(char in){
	if(in>='A' && in<='Z') return in-'A'+'a';
	if(in=='¥') return '¹';
	if(in=='¯') return '¿';
	if(in=='Œ') return 'œ';
	if(in=='') return 'Ÿ';
	if(in=='Ê') return 'ê';
	if(in=='Æ') return 'æ';
	if(in=='Ñ') return 'ñ';
	if(in=='Ó') return 'ó';
	if(in=='£') return '³';
	return in;
}

bool string_compare_lower(string s1, string s2){
	if(s1.length()!=s1.length()) return false;
	for(unsigned int i=0; i<s1.length(); i++){
		if(to_lowercase(s1[i])!=to_lowercase(s2[i])) return false;
	}
	return true;
}

bool string_begins(string s1, string s2){
    if(s1.length() < s2.length()) return false;
    for(unsigned int i=0; i<s2.length(); i++){
        if(s1.at(i) != s2.at(i)) return false;
    }
    return true;
}

void ss_clear(stringstream &sstream){
	sstream.str("");
	sstream.clear();
}

string get_time(){
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    stringstream ss;
    if(timeinfo->tm_hour<10) ss<<"0";
    ss<<timeinfo->tm_hour<<":";
    if(timeinfo->tm_min<10) ss<<"0";
    ss<<timeinfo->tm_min<<":";
    if(timeinfo->tm_sec<10) ss<<"0";
    ss<<timeinfo->tm_sec<<", ";
    if(timeinfo->tm_mday<10) ss<<"0";
    ss<<timeinfo->tm_mday<<".";
    if(timeinfo->tm_mon+1<10) ss<<"0";
    ss<<timeinfo->tm_mon+1<<".";
    ss<<timeinfo->tm_year+1900;
    return ss.str();
}
