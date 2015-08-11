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
