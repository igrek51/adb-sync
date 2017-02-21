//
// Created by igrek on 07/02/17.
//

#include "SystemCmdError.h"
#include <sstream>

SystemCmdError::SystemCmdError(string cmd) : Error() {
	this->cmd = cmd;
	errorCode = 0;
	output = "";
}

SystemCmdError::SystemCmdError(string cmd, int errorCode) : Error() {
	this->cmd = cmd;
	this->errorCode = errorCode;
	output = "";
}

SystemCmdError::SystemCmdError(string cmd, int errorCode, string output) : Error() {
	this->cmd = cmd;
	this->errorCode = errorCode;
	this->output = output;
}

SystemCmdError::~SystemCmdError() {}

string SystemCmdError::getMessage() {
	if (errorCode == 0) {
		return "failed executing command: " + cmd;
	} else {
		stringstream ss;
		ss << "failed executing command (exit code " << errorCode << "): " << cmd;
		if (!output.empty()) {
			ss << ", output:" << endl << output;
		}
		return ss.str();
	}
}



