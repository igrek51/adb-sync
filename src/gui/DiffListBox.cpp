//
// Created by igrek on 04/02/17.
//

#include "DiffListBox.h"
#include <QHeaderView>

DiffListBox::DiffListBox(QTableWidget* listWidget) {
	this->listWidget = listWidget;
	init();
}

DiffListBox::~DiffListBox() {

}

void DiffListBox::init() {
	for (int i = 0; i < 2; i++)
		listWidget->insertColumn(0);
	listWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

int DiffListBox::rowCount() {
	return listWidget->rowCount();
}

void DiffListBox::remove(int rowIndex) {
	listWidget->removeRow(rowIndex);
}

void DiffListBox::clear() {
	while (listWidget->rowCount() > 0)
		listWidget->removeRow(0);
}

int DiffListBox::selectedRowIndex() {
	QList<QTableWidgetItem*> selected = listWidget->selectedItems();
	if (selected.size() == 0)
		return -1;
	return listWidget->currentRow();
}

void DiffListBox::selectRow(int rowIndex) {
	if (rowIndex >= rowCount())
		rowIndex = -1;
	listWidget->setCurrentCell(rowIndex, 0);
}

void DiffListBox::update(vector<Diff*>* diffs) {
	clear();
	if (diffs->empty())
		return;

	int index = 1;
	for (Diff* diff : *diffs) {
		addDiff(diff);
		index++;
	}
}

void DiffListBox::addDiff(Diff* diff) {
	//TODO showing reversed diff direction
	addRow(Diff::diffTypeName(diff->type) + ":", diff->remoteFile);
}

void DiffListBox::addRow(string s1, string s2) {
	listWidget->insertRow(rowCount());
	QTableWidgetItem* item = new QTableWidgetItem(s1.c_str());
	item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	listWidget->setItem(rowCount() - 1, 0, item);
	item = new QTableWidgetItem(s2.c_str());
	item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	listWidget->setItem(rowCount() - 1, 1, item);
}
