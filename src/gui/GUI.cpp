//
// Created by igrek on 04/02/17.
//

#include "GUI.h"
#include "../events/ProgressUpdated.h"
#include "../dispatcher/EventDispatcher.h"
#include "../events/ShowUIMessageRequest.h"
#include "../events/DiffPartialScanCompleted.h"
#include "../events/DiffListUpdateRequest.h"

GUI::GUI() {
	mainwindow = new MainWindow();
	mainwindow->show();
	registerEvents();
}

GUI::~GUI() {
	delete mainwindow;
}

void GUI::registerEvents() {
	EventDispatcher::registerEventObserver<ProgressUpdated>(this);
	EventDispatcher::registerEventObserver<ShowUIMessageRequest>(this);
	EventDispatcher::registerEventObserver<DiffPartialScanCompleted>(this);
	EventDispatcher::registerEventObserver<DiffListUpdateRequest>(this);
}

void GUI::onEvent(Event* e) {
	if (e->instanceof<ProgressUpdated*>()) {
		mainwindow->setProgress(e->cast<ProgressUpdated*>()->progress);
	} else if (e->instanceof<ShowUIMessageRequest*>()) {
		mainwindow->uiMessage(e->cast<ShowUIMessageRequest*>()->message);
	} else if (e->instanceof<DiffPartialScanCompleted*>()) {
		Diff* newDiff = e->cast<DiffPartialScanCompleted*>()->newDiff;
		if (newDiff == nullptr) {
			// scanning finished
			mainwindow->setProgress(1);
			mainwindow->uiMessage("Difference scanning completed");
		} else {
			mainwindow->addDiff(newDiff);
		}
	} else if (e->instanceof<DiffListUpdateRequest*>()) {
		vector<Diff*>* diffs = e->cast<DiffListUpdateRequest*>()->diffs;
		mainwindow->updateDiffs(diffs);
	}
}