#include "bfd3me.h"
#include "ui_bfd3me.h"

#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>

#include "util.h"

#define HIDE(x) do {\
    ui->##x##Edit->hide();\
    ui->##x##Check->hide();\
    ui->##x##Lbl->hide();\
    } while (0)

#define SHOW(x) do {\
    ui->##x##Edit->show();\
    ui->##x##Check->show();\
    ui->##x##Lbl->show();\
    } while (0)

BFD3ME::~BFD3ME()
{
    delete ui;
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

void BFD3ME::setFilterStrings() {
    QStringList filters;
    filters << "No filter" << "Name" << "Library name" << "Library code";
    if (_type == Util::Kitpiece) {
        filters << "Class" << "Subclass" << "Manufacturer" << "Model" << "Date" << "Dimensions" << "Beater";
    }
    ui->comboBox->clear();
    ui->comboBox->insertItems(0, filters);
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

void BFD3ME::on_loadBtn_clicked()
{
    if (!loadThread.isRunning() && ui->pathEdit->text() != "") {
        loadThread.start();
    }
}

void BFD3ME::on_selection_changed()
{
    QModelIndexList list = ui->itemlist->selectionModel()->selectedIndexes();
    if (list.empty()) {
        clearAll();
        return;
    }
    bool first = true;
    switch (_type) {
    case Util::Kit:
        foreach (QModelIndex i, list) {
            QSharedPointer<Kit> k = kmodel.getItem(kfmodel.mapToSource(i).row());
            setText(k->getName(), ui->nameEdit, first);
            setText(k->getLibname(), ui->libnameEdit, first);
            setText(k->getLibcode(), ui->libcodeEdit, first);
            first = false;
        }
        break;
    case Util::Preset:
        foreach (QModelIndex i, list) {
            QSharedPointer<Preset> p = pmodel.getItem(pfmodel.mapToSource(i).row());
            setText(p->getName(), ui->nameEdit, first);
            setText(p->getLibname(), ui->libnameEdit, first);
            setText(p->getLibcode(), ui->libcodeEdit, first);
            first = false;
        }
        break;
    case Util::Kitpiece:
        foreach (QModelIndex i, list) {
            QSharedPointer<Kitpiece> kp = kpmodel.getItem(kpfmodel.mapToSource(i).row());
            setText(kp->getName(), ui->nameEdit, first);
            setText(kp->getLibname(), ui->libnameEdit, first);
            setText(kp->getLibcode(), ui->libcodeEdit, first);
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

void BFD3ME::on_comboBox_currentIndexChanged(int index)
{
    Util::FilterType filter_type = (Util::FilterType) index;

    switch (_type) {
    case Util::Kit:
        kfmodel.setFilterType(filter_type);
        kfmodel.invalidate();
        break;
    case Util::Kitpiece:
        kpfmodel.setFilterType(filter_type);
        kpfmodel.invalidate();
        break;
    case Util::Preset:
        pfmodel.setFilterType(filter_type);
        pfmodel.invalidate();
        break;
    }
}

void BFD3ME::on_lineEdit_textChanged(const QString &arg1)
{
    switch (_type) {
    case Util::Kit:
        kfmodel.setFilterFixedString(arg1);
        kfmodel.invalidate();
        break;
    case Util::Kitpiece:
        kpfmodel.setFilterFixedString(arg1);
        kpfmodel.invalidate();
        break;
    case Util::Preset:
        pfmodel.setFilterFixedString(arg1);
        pfmodel.invalidate();
        break;
    }
}
