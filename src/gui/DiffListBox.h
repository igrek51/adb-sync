//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_DIFFLISTBOX_H
#define ADBSYNC_DIFFLISTBOX_H

#include <QTableWidget>
#include <vector>
#include <string>
#include "../diffs/Diff.h"

using namespace std;

class DiffListBox {
public:
    DiffListBox(QTableWidget* listWidget);

    ~DiffListBox();

    void init();

    int rowCount();

    void remove(int rowIndex);

    void clear();

    int currentIndex();

    void selectRow(int rowIndex);

    void addDiff(Diff* diff);

    void addRow(string s1, string s2);

    void update(vector<Diff*>* diffs);

private:
    QTableWidget* listWidget;
};


#endif //ADBSYNC_DIFFLISTBOX_H
