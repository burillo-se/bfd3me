/***
* BFD3 Metadata Editor
* Copyright (C) 2015 Anatoly Burakov
* 
* Contact information: burakov.anatoly@gmail.com
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#include "bfd3me.h"
#include "ui_bfd3me.h"

#include <QMessageBox>
#include <QStandardPaths>

#include "utils/util.h"

/*
 * Source file with various utility functions for window class
 */

static QString getKey(Util::Mode mode, Util::Type type) {
    QString key;
    switch (type) {
    case Util::Kit:
        key += "kit";
        break;
    case Util::Kitpiece:
        key += "kitpiece";
        break;
    case Util::Preset:
        key += "preset";
        break;
    }
    switch (mode) {
    case Util::Database:
        key += "db";
        break;
    case Util::Files:
        key += "f";
        break;
    }
    return key;
}

static QString getDBPath(QString fname) {
    // first, try regular path
    QString result = QStandardPaths::locate(QStandardPaths::AppDataLocation, QString("../fxpansion/BFD3/%0").arg(fname));
    if (result != "")
        return result;

    // on OSX (at least in my virtual machine), that code didn't seem to work, so let's try alternate method
    return QStandardPaths::locate(QStandardPaths::HomeLocation, QString("Library/Application Support/fxpansion/BFD3/%0").arg(fname));
}

QStringList BFD3ME::getPastPaths() {
    QStringList paths;
    QString key = getKey(_mode, _type);

    int size = settings.beginReadArray(key);
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        QString path = settings.value("path").toString();
        paths << path;
    }
    settings.endArray();
    return paths;
}


void BFD3ME::setNewPastPath() {
    QString curPath = ui->pathEdit->currentText();
    QStringList paths;
    QString key = getKey(_mode, _type);

    paths << curPath;
    QStringList prevPaths = getPastPaths();

    // don't save duplicates
    for (int i = 0; i < prevPaths.count(); i++) {
        if (prevPaths.at(i) != curPath)
            paths << prevPaths.at(i);
    }

    while (paths.size() > 10) {
        paths.removeLast();
    }

    settings.beginWriteArray(key, paths.size());
    for (int i = 0; i < paths.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("path", paths.at(i));
    }
    settings.endArray();
}

void BFD3ME::setPathEditItems() {
    QStringList paths;

    if (_mode == Util::Database) {
        QString autoPath;
        switch(_type) {
        case Util::Kit:
            autoPath = getDBPath(KIT_DB_FILENAME);
            break;
        case Util::Kitpiece:
            autoPath = getDBPath(KITPIECE_DB_FILENAME);
            break;
        case Util::Preset:
            autoPath = getDBPath(PRESET_DB_FILENAME);
            break;
        }
        if (autoPath != "") {
            QFileInfo fi(autoPath);
            paths << QDir::toNativeSeparators(fi.absoluteFilePath());
        }
    }
    paths.append(getPastPaths());
    ui->pathEdit->clear();
    ui->pathEdit->addItems(paths);
    ui->pathEdit->setCurrentIndex(0);
}

void BFD3ME::clearData() {
    disconnect(&kselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selected()));
    disconnect(&kpselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selected()));
    disconnect(&pselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selected()));
    kselection.clearSelection();
    kpselection.clearSelection();
    pselection.clearSelection();
    kit_f.clear();
    kitpiece_f.clear();
    preset_f.clear();
    kit_db.clear();
    kitpiece_db.clear();
    preset_db.clear();
    kmodel.clearList();
    kpmodel.clearList();
    pmodel.clearList();
    kfmodel.invalidate();
    kpfmodel.invalidate();
    pfmodel.invalidate();
    ui->itemlist->setModel(0);
}

void BFD3ME::setMode(Util::Mode mode) {
    _mode = mode;

    clearAll();
    clearData();
    setPathEditItems();
    setEnabledButtons();
}

void BFD3ME::setType(Util::Type type) {
    _type = type;

    clearAll();
    clearData();
    setPathEditItems();
    displayEdits();
    setFilterStrings();
    setEnabledButtons();
}

void BFD3ME::setText(const QString &text, QLineEdit *l, bool first) {
    if (first) {
        l->setText(text);
    } else {
        if (l->text() != text) {
            l->setText("<Multiple values>");
        }
    }
}

void BFD3ME::setFilterStrings() {
    QStringList filters;
    filters << "No filter" << "Name" << "Library name" << "Library code";
    if (_type == Util::Kitpiece) {
        filters << "Class" << "Subclass" << "Manufacturer" << "Model" << "Date" << "Dimensions" << "Beater";
    }
    ui->comboBox->clear();
    ui->comboBox->insertItems(0, filters);
}
