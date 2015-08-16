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

#include <QStandardPaths>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

#include "utils/util.h"
#include "utils/boilerplate.h"

/*
 * Source file with various boring boilerplate code
 */

#define HIDE(x) do {\
    ui->CONCAT(x, Edit)->hide();\
    ui->CONCAT(x, Check)->hide();\
    ui->CONCAT(x, Lbl)->hide();\
    } while (0)

#define SHOW(x) do {\
    ui->CONCAT(x, Edit)->show();\
    ui->CONCAT(x, Check)->show();\
    ui->CONCAT(x, Lbl)->show();\
    } while (0)

BFD3ME::~BFD3ME()
{
    delete ui;
}

void BFD3ME::on_actionAbout_triggered() {
    QMessageBox::about(this, "About BFD3 Metadata Editor",
                       "Copyright (C) Anatoly Burakov, 2015\n\n"
                       "BFD and FXpansion are trademarks of FXpansion Audio UK Ltd.\n\n"
                       "Usage of this program is not endorsed or supported by FXpansion in any way.\n\n"
                       "This program is Free Software licensed under GPLv3 or later.");
}

void BFD3ME::on_actionAbout_Qt_triggered() {
    QMessageBox::aboutQt(this, "Qt");
}

void BFD3ME::clearAll() {
    ui->nameEdit->clear();
    ui->libnameEdit->clear();
    ui->libcodeEdit->clear();
    ui->classEdit->clear();
    ui->subclassEdit->clear();
    ui->manufacturerEdit->clear();
    ui->modelEdit->clear();
    ui->dateEdit->clear();
    ui->dimensionsEdit->clear();
    ui->beaterEdit->clear();
}

void BFD3ME::displayEdits() {
    SHOW(name);
    SHOW(libname);
    SHOW(libcode);
    if (_type != Util::Kitpiece) {
        HIDE(class);
        HIDE(subclass);
        HIDE(manufacturer);
        HIDE(model);
        HIDE(date);
        HIDE(dimensions);
        HIDE(beater);
    } else {
        SHOW(class);
        SHOW(subclass);
        SHOW(manufacturer);
        SHOW(model);
        SHOW(date);
        SHOW(dimensions);
        SHOW(beater);
    }
}

void BFD3ME::on_mode_f_toggled(bool checked)
{
    if (checked) {
        setMode(Util::Files);
    }
}

void BFD3ME::on_mode_db_toggled(bool checked)
{
    if (checked) {
        setMode(Util::Database);
    }
}

void BFD3ME::on_type_kitpiece_toggled(bool checked)
{
    if (checked) {
        setType(Util::Kitpiece);
    }
}

void BFD3ME::on_type_kit_toggled(bool checked)
{
    if (checked) {
        setType(Util::Kit);
    }
}

void BFD3ME::on_type_preset_toggled(bool checked)
{
    if (checked) {
        setType(Util::Preset);
    }
}

void BFD3ME::setEnabledButtons() {
    if (ui->pathEdit->text() != "") {
        ui->loadBtn->setEnabled(true);
        ui->saveBtn->setEnabled(true);
        ui->restoreBtn->setEnabled(true);
        if (_mode == Util::Database)
            ui->deleteBtn->setEnabled(true);
        else
            ui->deleteBtn->setEnabled(false);
    } else {
        ui->loadBtn->setEnabled(false);
        ui->saveBtn->setEnabled(false);
        ui->restoreBtn->setEnabled(false);
        ui->deleteBtn->setEnabled(false);
    }
}

void BFD3ME::on_pathEdit_textChanged(const QString &)
{
    setEnabledButtons();
}


#define SELECTION_START(x) \
    list = CONCAT(x, selection).selectedIndexes(); \
    if (list.empty()) { \
        clearAll(); \
        return; \
    }
#define SELECTION_BOILERPLATE(classname, x) \
    QSharedPointer<classname> (x) = CONCAT(x, model).getItem(CONCAT(x, fmodel).mapToSource(i)); \
    setText((x)->getName(), ui->nameEdit, first); \
    setText((x)->getLibname(), ui->libnameEdit, first); \
    setText((x)->getLibcode(), ui->libcodeEdit, first);

#define SELECTION_CHANGED(classname, x) \
    SELECTION_START(x) \
    foreach (QModelIndex i, list) { \
        SELECTION_BOILERPLATE(classname, x) \
        first = false; \
    }


void BFD3ME::selected()
{
    QModelIndexList list;
    bool first = true;
    switch (_type) {
    case Util::Kit:
        SELECTION_CHANGED(Kit, k)
        break;
    case Util::Preset:
        SELECTION_CHANGED(Preset, p)
        break;
    case Util::Kitpiece:
        SELECTION_START(kp)
        foreach (QModelIndex i, list) {
            SELECTION_BOILERPLATE(Kitpiece, kp)
            setText(kp->getClass(), ui->classEdit, first);
            setText(kp->getSubclass(), ui->subclassEdit, first);
            setText(kp->getManufacturer(), ui->manufacturerEdit, first);
            setText(kp->getModel(), ui->modelEdit, first);
            setText(kp->getDate(), ui->dateEdit, first);
            setText(kp->getDimensions(), ui->dimensionsEdit, first);
            setText(kp->getBeater(), ui->beaterEdit, first);
            first = false;
        }
        break;
    }
}

void BFD3ME::on_browseBtn_clicked()
{
    QFileDialog dialog;
    if (_mode == Util::Database) {
        dialog.setFileMode(QFileDialog::ExistingFile);
        switch (_type) {
        case Util::Kit:
            dialog.setNameFilter(KIT_DB_FILENAME);
            break;
        case Util::Kitpiece:
            dialog.setNameFilter(KITPIECE_DB_FILENAME);
            break;
        case Util::Preset:
            dialog.setNameFilter(PRESET_DB_FILENAME);
            break;
        }
    } else {
        dialog.setFileMode(QFileDialog::Directory);
        dialog.setOption(QFileDialog::ShowDirsOnly);
    }
    dialog.setReadOnly(true);
    dialog.exec();
    if (dialog.result() == QDialog::Accepted) {
        ui->pathEdit->setText(QDir::toNativeSeparators(dialog.selectedFiles().at(0)));
    }
}

#define FILTER_BOILERPLATE(x) \
    do { \
    CONCAT(x, fmodel).setFilterType(filter_type); \
    CONCAT(x, fmodel).invalidate(); \
    } while (0);

void BFD3ME::on_comboBox_currentIndexChanged(int index)
{
    Util::FilterType filter_type = (Util::FilterType) index;

    switch (_type) {
    case Util::Kit:
        FILTER_BOILERPLATE(k)
        break;
    case Util::Kitpiece:
        FILTER_BOILERPLATE(kp)
        break;
    case Util::Preset:
        FILTER_BOILERPLATE(p)
        break;
    }
}

#define TEXT_BOILERPLATE(x) \
    do { \
    CONCAT(x, fmodel).setFilterFixedString(arg1); \
    CONCAT(x, fmodel).invalidate(); \
    } while (0);

void BFD3ME::on_lineEdit_textChanged(const QString &arg1)
{
    switch (_type) {
    case Util::Kit:
        TEXT_BOILERPLATE(k)
        break;
    case Util::Kitpiece:
        TEXT_BOILERPLATE(kp)
        break;
    case Util::Preset:
        TEXT_BOILERPLATE(kp)
        break;
    }
}
