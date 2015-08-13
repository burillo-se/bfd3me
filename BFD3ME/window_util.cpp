#include "bfd3me.h"
#include "ui_bfd3me.h"

#include <QMessageBox>
#include <QStandardPaths>

#include "utils/util.h"

/*
 * Source file with various utility functions for window class
 */

void BFD3ME::setDefaultDatabasePath() {
    QFileInfo fi;
    switch (_type) {
    case Util::Kitpiece:
        fi = QFileInfo(QStandardPaths::locate(QStandardPaths::AppDataLocation, "../FXpansion/BFD3/" KITPIECE_DB_FILENAME));
        ui->pathEdit->setText(QDir::toNativeSeparators(fi.absoluteFilePath()));
        break;
    case Util::Kit:
        fi = QFileInfo(QStandardPaths::locate(QStandardPaths::AppDataLocation, "../FXpansion/BFD3/" KIT_DB_FILENAME));
        ui->pathEdit->setText(QDir::toNativeSeparators(fi.absoluteFilePath()));
        break;
    case Util::Preset:
        fi = QFileInfo(QStandardPaths::locate(QStandardPaths::AppDataLocation, "../FXpansion/BFD3/" PRESET_DB_FILENAME));
        ui->pathEdit->setText(QDir::toNativeSeparators(fi.absoluteFilePath()));
        break;
    }
}

void BFD3ME::clearData() {
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
