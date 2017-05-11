//
// Created by igrek on 04/02/17.
//

#include "Diff.h"

Diff::Diff(string localFile, string remoteFile, DiffType type) {
	this->localFile = localFile;
	this->remoteFile = remoteFile;
	this->type = type;
	this->inverted = false;
}

string Diff::typeName() {
	if (!inverted) { // synchronize from local to remote
		switch (type) {
			case DiffType::NO_DIRECTORY:
				return "not existing directory";
			case DiffType::NO_REGULAR_FILE:
				return "not existing file";
			case DiffType::DIFFERENT_CONTENT:
				return "different content";
			case DiffType::DIFFERENT_SIZE:
				return "different size";
			case DiffType::REDUNDANT_DIRECTORY:
				return "reduntant directory";
			case DiffType::REDUNDANT_REGULAR_FILE:
				return "redundant file";
		}
	} else { // inverted synchronization direction: from remote to local
		switch (type) {
			case DiffType::NO_DIRECTORY: // no remote directory
				return "redundant directory";
			case DiffType::NO_REGULAR_FILE: // no remote file
				return "redundant file";
			case DiffType::DIFFERENT_CONTENT: // different checksums
				return "different content";
			case DiffType::DIFFERENT_SIZE: // different file sizes
				return "different size";
			case DiffType::REDUNDANT_DIRECTORY: // no local directory
				return "not existing directory";
			case DiffType::REDUNDANT_REGULAR_FILE: // no local file
				return "not existing file";
		}
	}
	return "";
}
