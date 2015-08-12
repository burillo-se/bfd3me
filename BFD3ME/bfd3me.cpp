#include "bfd3me.h"
#include "ui_bfd3me.h"

#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include "util.h"

BFD3ME::BFD3ME(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BFD3ME),
    kit_f(KIT_TAG, KIT_FILTER),
    kitpiece_f(KITPIECE_TAG, KITPIECE_FILTER),
    preset_f(PRESET_TAG, PRESET_FILTER),
    kit_db(KIT_TAG),
    kitpiece_db(KITPIECE_TAG),
    preset_db(PRESET_TAG)
{
    ui->setupUi(this);
    setMode(Util::Files);
    setType(Util::Kitpiece);

    // set up thread signals
    connect(&loadThread, SIGNAL(started()), this, SLOT(load()));
    connect(&loadThread, SIGNAL(finished()), ui->statusBar, SLOT(clearMessage()));

    // set up Helper signals to communicate with the thread
    connect(&kit_f, SIGNAL(progressChanged(QString,int,int)), this, SLOT(progressChanged(QString,int,int)));
    connect(&kit_f, SIGNAL(finished()), &loadThread, SLOT(quit()));
    connect(&kitpiece_f, SIGNAL(progressChanged(QString,int,int)), this, SLOT(progressChanged(QString,int,int)));
    connect(&kitpiece_f, SIGNAL(finished()), &loadThread, SLOT(quit()));
    connect(&preset_f, SIGNAL(progressChanged(QString,int,int)), this, SLOT(progressChanged(QString,int,int)));
    connect(&preset_f, SIGNAL(finished()), &loadThread, SLOT(quit()));

    // set up DBHelper signals to communicate with the thread
    connect(&kit_db, SIGNAL(progressChanged(QString,int,int)), this, SLOT(progressChanged(QString,int,int)));
    connect(&kit_db, SIGNAL(finished()), &loadThread, SLOT(quit()));
    connect(&kitpiece_db, SIGNAL(progressChanged(QString,int,int)), this, SLOT(progressChanged(QString,int,int)));
    connect(&kitpiece_db, SIGNAL(finished()), &loadThread, SLOT(quit()));
    connect(&preset_db, SIGNAL(progressChanged(QString,int,int)), this, SLOT(progressChanged(QString,int,int)));
    connect(&preset_db, SIGNAL(finished()), &loadThread, SLOT(quit()));
}

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

void BFD3ME::setMode(Util::Mode mode) {
    _mode = mode;
    kmodel.clearList();
    kpmodel.clearList();
    pmodel.clearList();
    ui->pathEdit->clear();
    clearAll();
    if (_mode == Util::Database) {
        setDefaultDatabasePath();
        ui->deleteBtn->setEnabled(true);
    } else {
        ui->deleteBtn->setEnabled(false);
    }
}

void BFD3ME::setType(Util::Type type) {
    _type = type;
    kmodel.clearList();
    kpmodel.clearList();
    pmodel.clearList();
    clearAll();
    displayEdits();
    if (_mode == Util::Database) {
        setDefaultDatabasePath();
    }
    setFilterStrings();
}

BFD3ME::~BFD3ME()
{
    delete ui;
}

void BFD3ME::on_comboBox_currentIndexChanged(int index)
{
    Util::FilterType filter_type = (Util::FilterType) index;
}

void BFD3ME::progressChanged(QString progressStr, int progressDone, int progressTodo) {
    ui->statusBar->showMessage(QString("%0: %1/%2").arg(progressStr)
                               .arg(progressDone).arg(progressTodo));
}

/*
 * This runs in a separate thread
 */
void BFD3ME::load() {
    disconnect(ui->itemlist->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    if (_mode == Util::Database) {
        switch (_type) {
        case Util::Kit:
            kmodel.setList(kit_db.load(ui->pathEdit->text()));
            fmodel.setSourceModel(&kmodel);
            break;
        case Util::Kitpiece:
            kpmodel.setList(kitpiece_db.load(ui->pathEdit->text()));
            fmodel.setSourceModel(&kpmodel);
            break;
        case Util::Preset:
            pmodel.setList(preset_db.load(ui->pathEdit->text()));
            fmodel.setSourceModel(&pmodel);
            break;
        }
    } else {
        switch (_type) {
        case Util::Kit:
            kmodel.setList(kit_f.load(ui->pathEdit->text()));
            fmodel.setSourceModel(&kmodel);
            break;
        case Util::Kitpiece:
            kpmodel.setList(kitpiece_f.load(ui->pathEdit->text()));
            fmodel.setSourceModel(&kpmodel);
            break;
        case Util::Preset:
            pmodel.setList(preset_f.load(ui->pathEdit->text()));
            fmodel.setSourceModel(&pmodel);
            break;
        }
    }
    ui->itemlist->setModel(&fmodel);
    connect(ui->itemlist->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
}

void BFD3ME::on_loadBtn_clicked()
{
    if (!loadThread.isRunning() && ui->pathEdit->text() != "") {
        loadThread.start();
    }
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

void BFD3ME::on_selection_changed()
{
    QModelIndexList list = ui->itemlist->selectionModel()->selectedIndexes();
    bool first = true;
    switch (_type) {
    case Util::Kit:
        foreach (QModelIndex i, list) {
            QSharedPointer<Kit> k = kmodel.getItem(fmodel.mapToSource(i).row());
            setText(k->getName(), ui->nameEdit, first);
            setText(k->getLibname(), ui->libnameEdit, first);
            setText(k->getLibcode(), ui->libcodeEdit, first);
            first = false;
        }
        break;
    case Util::Preset:
        foreach (QModelIndex i, list) {
            QSharedPointer<Preset> p = pmodel.getItem(fmodel.mapToSource(i).row());
            setText(p->getName(), ui->nameEdit, first);
            setText(p->getLibname(), ui->libnameEdit, first);
            setText(p->getLibcode(), ui->libcodeEdit, first);
            first = false;
        }
        break;
    case Util::Kitpiece:
        foreach (QModelIndex i, list) {
            QSharedPointer<Kitpiece> kp = kpmodel.getItem(fmodel.mapToSource(i).row());
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

void BFD3ME::on_restoreBtn_clicked()
{
    // bail out if no data
    if (!ui->itemlist->model())
        return;

    // if database, we're restoring everything at once
    if (_mode == Util::Database) {
        switch (_type) {
        case Util::Kit:
            kmodel.setList(kit_db.restoreFromBackup());
            break;
        case Util::Kitpiece:
            kpmodel.setList(kitpiece_db.restoreFromBackup());
            break;
        case Util::Preset:
            pmodel.setList(preset_db.restoreFromBackup());
            break;
        }
    // otherwise, restore each selected file
    } else {
        QModelIndexList list = ui->itemlist->selectionModel()->selectedIndexes();
        switch (_type) {
        case Util::Kit:
            foreach (QModelIndex i, list) {
                int idx = fmodel.mapToSource(i).row();
                QSharedPointer<Kit> k = kmodel.getItem(idx);
                k = kit_f.restoreFromBackup(k);
                kmodel.setItem(k, idx);
            }
            break;
        case Util::Preset:
            foreach (QModelIndex i, list) {
                int idx = fmodel.mapToSource(i).row();
                QSharedPointer<Preset> p = pmodel.getItem(idx);
                pmodel.setItem(preset_f.restoreFromBackup(p), idx);
            }
            break;
        case Util::Kitpiece:
            foreach (QModelIndex i, list) {
                int idx = fmodel.mapToSource(i).row();
                QSharedPointer<Kitpiece> kp = kpmodel.getItem(idx);
                kpmodel.setItem(kitpiece_f.restoreFromBackup(kp), idx);
            }
            break;
        }
    }
}

void BFD3ME::on_deleteBtn_clicked()
{
    // bail out if no data
    if (!ui->itemlist->model())
        return;

    // we already know we're in database mode, so check type only
    QModelIndexList list = ui->itemlist->selectionModel()->selectedIndexes();
    switch (_type) {
    case Util::Kit:
        // we need to go from the end, otherwise it'll end in disaster
        for (int s_idx = list.count() - 1; s_idx >= 0; s_idx--) {
            QModelIndex i = list[s_idx];
            int idx = fmodel.mapToSource(i).row();
            QSharedPointer<Kit> k = kmodel.getItem(idx);
            kmodel.removeItem(idx);
            kit_db.remove(k);
        }
        break;
    case Util::Preset:
        // we need to go from the end, otherwise it'll end in disaster
        for (int s_idx = list.count() - 1; s_idx >= 0; s_idx--) {
            QModelIndex i = list[s_idx];
            int idx = fmodel.mapToSource(i).row();
            QSharedPointer<Preset> p = pmodel.getItem(idx);
            pmodel.removeItem(idx);
            preset_db.remove(p);
        }
        break;
    case Util::Kitpiece:
        // we need to go from the end, otherwise it'll end in disaster
        for (int s_idx = list.count() - 1; s_idx >= 0; s_idx--) {
            QModelIndex i = list[s_idx];
            int idx = fmodel.mapToSource(i).row();
            QSharedPointer<Kitpiece> kp = kpmodel.getItem(idx);
            kpmodel.removeItem(idx);
            kitpiece_db.remove(kp);
        }
        break;
    }
}

void BFD3ME::on_saveBtn_clicked()
{
    // bail out if no data
    if (!ui->itemlist->model())
        return;

    // update our data
    QModelIndexList list = ui->itemlist->selectionModel()->selectedIndexes();
    foreach (QModelIndex i, list) {
        int idx = fmodel.mapToSource(i).row();
        switch (_type) {
        case Util::Kitpiece:
            {
                QSharedPointer<Kitpiece> kp = kpmodel.getItem(idx);
                if (ui->nameCheck->isChecked())
                    kp->setName(ui->nameEdit->text());
                if (ui->libcodeCheck->isChecked())
                    kp->setLibcode(ui->libcodeEdit->text());
                if (ui->libnameCheck->isChecked())
                    kp->setLibname(ui->libnameEdit->text());
                if (ui->classCheck->isChecked())
                    kp->setClass(ui->classEdit->text());
                if (ui->subclassCheck->isChecked())
                    kp->setSubclass(ui->subclassEdit->text());
                if (ui->manufacturerCheck->isChecked())
                    kp->setManufacturer(ui->manufacturerEdit->text());
                if (ui->modelCheck->isChecked())
                    kp->setModel(ui->modelEdit->text());
                if (ui->dateCheck->isChecked())
                    kp->setDate(ui->dateEdit->text());
                if (ui->dimensionsCheck->isChecked())
                    kp->setDimensions(ui->dimensionsEdit->text());
                if (ui->beaterCheck->isChecked())
                    kp->setBeater(ui->beaterEdit->text());
            }
            break;
        case Util::Kit:
            {
                QSharedPointer<Kit> k = kmodel.getItem(idx);
                if (ui->nameCheck->isChecked())
                    k->setName(ui->nameEdit->text());
                if (ui->libcodeCheck->isChecked())
                    k->setLibcode(ui->libcodeEdit->text());
                if (ui->libnameCheck->isChecked())
                    k->setLibname(ui->libnameEdit->text());
            }
            break;
        case Util::Preset:
            {
                QSharedPointer<Preset> p = pmodel.getItem(idx);
                if (ui->nameCheck->isChecked())
                    p->setName(ui->nameEdit->text());
                if (ui->libcodeCheck->isChecked())
                    p->setLibcode(ui->libcodeEdit->text());
                if (ui->libnameCheck->isChecked())
                    p->setLibname(ui->libnameEdit->text());
            }
            break;
        }
    }

    // if database, we're restoring everything at once
    if (_mode == Util::Database) {
        switch (_type) {
        case Util::Kit:
            kit_db.save();
            break;
        case Util::Kitpiece:
            kitpiece_db.save();
            break;
        case Util::Preset:
            preset_db.save();
            break;
        }
    // otherwise, restore each selected file
    } else {
        switch (_type) {
        case Util::Kit:
            foreach (QModelIndex i, list) {
                int idx = fmodel.mapToSource(i).row();
                QSharedPointer<Kit> k = kmodel.getItem(idx);
                kit_f.save(k);
            }
            break;
        case Util::Preset:
            foreach (QModelIndex i, list) {
                int idx = fmodel.mapToSource(i).row();
                QSharedPointer<Preset> p = pmodel.getItem(idx);
                preset_f.save(p);
            }
            break;
        case Util::Kitpiece:
            foreach (QModelIndex i, list) {
                int idx = fmodel.mapToSource(i).row();
                QSharedPointer<Kitpiece> kp = kpmodel.getItem(idx);
                kitpiece_f.save(kp);
            }
            break;
        }
    }
}
