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

static QString getDBPath(QString fname) {
    // first, try regular path
    QString result = QStandardPaths::locate(QStandardPaths::AppDataLocation, QString("../fxpansion/BFD3/%0").arg(fname));
    if (result != "")
        return result;

    // on OSX (at least in my virtual machine), that code didn't seem to work, so let's try alternate method
    return QStandardPaths::locate(QStandardPaths::HomeLocation, QString("Library/Application Support/fxpansion/BFD3/%0").arg(fname));
}

void BFD3ME::setDefaultDatabasePath() {
    QFileInfo fi;
    QString path;
    switch (_type) {
    case Util::Kitpiece:
        path = getDBPath(KITPIECE_DB_FILENAME);
        if (path == "")
            return;
        fi = QFileInfo(path);
        ui->pathEdit->setText(QDir::toNativeSeparators(fi.absoluteFilePath()));
        break;
    case Util::Kit:
        path = getDBPath(KIT_DB_FILENAME);
        if (path == "")
            return;
        fi = QFileInfo(path);
        ui->pathEdit->setText(QDir::toNativeSeparators(fi.absoluteFilePath()));
        break;
    case Util::Preset:
        path = getDBPath(PRESET_DB_FILENAME);
        if (path == "")
            return;
        fi = QFileInfo(path);
        ui->pathEdit->setText(QDir::toNativeSeparators(fi.absoluteFilePath()));
        break;
    }
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

    ui->pathEdit->clear();
    clearAll();
    clearData();
    if (_mode == Util::Database) {
        setDefaultDatabasePath();
    }
    setEnabledButtons();
}

void BFD3ME::setType(Util::Type type) {
    _type = type;

    clearAll();
    clearData();
    displayEdits();
    if (_mode == Util::Database) {
        setDefaultDatabasePath();
    }
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
