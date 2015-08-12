#include "bfd3me.h"
#include "ui_bfd3me.h"

#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

#include "util.h"

/*
 * Source file with main window functions (load/save/restore/delete)
 */

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
    connect(&kit_f, SIGNAL(error(QString,QString)), this, SLOT(error(QString,QString)));
    connect(&kit_f, SIGNAL(finished()), this, SLOT(finished()));

    connect(&kitpiece_f, SIGNAL(progressChanged(QString,int,int)), this, SLOT(progressChanged(QString,int,int)));
    connect(&kitpiece_f, SIGNAL(error(QString,QString)), this, SLOT(error(QString,QString)));
    connect(&kitpiece_f, SIGNAL(finished()), this, SLOT(finished()));

    connect(&preset_f, SIGNAL(progressChanged(QString,int,int)), this, SLOT(progressChanged(QString,int,int)));
    connect(&preset_f, SIGNAL(error(QString,QString)), this, SLOT(error(QString,QString)));
    connect(&preset_f, SIGNAL(finished()), this, SLOT(finished()));

    // set up DBHelper signals to communicate with the thread
    connect(&kit_db, SIGNAL(progressChanged(QString,int,int)), this, SLOT(progressChanged(QString,int,int)));
    connect(&kit_db, SIGNAL(error(QString,QString)), this, SLOT(error(QString,QString)));
    connect(&kit_db, SIGNAL(finished()), this, SLOT(finished()));

    connect(&kitpiece_db, SIGNAL(progressChanged(QString,int,int)), this, SLOT(progressChanged(QString,int,int)));
    connect(&kitpiece_db, SIGNAL(error(QString,QString)), this, SLOT(error(QString,QString)));
    connect(&kitpiece_db, SIGNAL(finished()), this, SLOT(finished()));

    connect(&preset_db, SIGNAL(progressChanged(QString,int,int)), this, SLOT(progressChanged(QString,int,int)));
    connect(&preset_db, SIGNAL(error(QString,QString)), this, SLOT(error(QString,QString)));
    connect(&preset_db, SIGNAL(finished()), this, SLOT(finished()));
}


/*
 * This runs in a separate thread
 */
void BFD3ME::load() {
    errors.clear();
    disconnect(ui->itemlist->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    switch (_type) {
    case Util::Kit:
        if (_mode == Util::Database)
            kmodel.setList(kit_db.load(ui->pathEdit->text()));
        else
            kmodel.setList(kit_f.load(ui->pathEdit->text()));
        kfmodel.setSourceModel(&kmodel);
        ui->itemlist->setModel(&kfmodel);
        kfmodel.invalidate();
        break;
    case Util::Kitpiece:
        if (_mode == Util::Database)
            kpmodel.setList(kitpiece_db.load(ui->pathEdit->text()));
        else
            kpmodel.setList(kitpiece_f.load(ui->pathEdit->text()));
        kpfmodel.setSourceModel(&kpmodel);
        ui->itemlist->setModel(&kpfmodel);
        kpfmodel.invalidate();
        break;
    case Util::Preset:
        if (_mode == Util::Database)
            pmodel.setList(preset_db.load(ui->pathEdit->text()));
        else
            pmodel.setList(preset_f.load(ui->pathEdit->text()));
        pfmodel.setSourceModel(&pmodel);
        ui->itemlist->setModel(&pfmodel);
        pfmodel.invalidate();
        break;
    }
    connect(ui->itemlist->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
}

void BFD3ME::on_restoreBtn_clicked()
{
    // bail out if no data
    if (!ui->itemlist->model())
        return;

    QList<int> idxs = getIdxList();

    // if database, we're restoring everything at once
    if (_mode == Util::Database) {
        switch (_type) {
        case Util::Kit:
            kmodel.setList(kit_db.restoreFromBackup());
            kfmodel.invalidate();
            break;
        case Util::Kitpiece:
            kpmodel.setList(kitpiece_db.restoreFromBackup());
            kpfmodel.invalidate();
            break;
        case Util::Preset:
            pmodel.setList(preset_db.restoreFromBackup());
            pfmodel.invalidate();
            break;
        }
    // otherwise, restore each selected file
    } else {
        switch (_type) {
        case Util::Kit:
            for (int i = 0; i < idxs.count(); i++) {
                int idx = idxs[i];
                QSharedPointer<Kit> k = kmodel.getItem(idx);
                kmodel.setItem(kit_f.restoreFromBackup(k), idx);
            }
            kfmodel.invalidate();
            break;
        case Util::Preset:
            for (int i = 0; i < idxs.count(); i++) {
                int idx = idxs[i];
                QSharedPointer<Preset> p = pmodel.getItem(idx);
                pmodel.setItem(preset_f.restoreFromBackup(p), idx);
            }
            pfmodel.invalidate();
            break;
        case Util::Kitpiece:
            for (int i = 0; i < idxs.count(); i++) {
                int idx = idxs[i];
                QSharedPointer<Kitpiece> kp = kpmodel.getItem(idx);
                kpmodel.setItem(kitpiece_f.restoreFromBackup(kp), idx);
            }
            kpfmodel.invalidate();
            break;
        }
    }
    ui->itemlist->selectionModel()->clearSelection();
}

void BFD3ME::on_deleteBtn_clicked()
{
    // bail out if no data
    if (!ui->itemlist->model())
        return;

    QList<int> idxs = getIdxList();

    // we already know we're in database mode, so check type only
    switch (_type) {
    case Util::Kit:
        // we need to go from the end, otherwise it'll end in disaster
        for (int s_idx = idxs.count() - 1; s_idx >= 0; s_idx--) {
            int idx = idxs[s_idx];
            QSharedPointer<Kit> k = kmodel.getItem(idx);
            kmodel.removeItem(idx);
            kit_db.remove(k);
            kfmodel.invalidate();
        }
        break;
    case Util::Preset:
        // we need to go from the end, otherwise it'll end in disaster
        for (int s_idx = idxs.count() - 1; s_idx >= 0; s_idx--) {
            int idx = idxs[s_idx];
            QSharedPointer<Preset> p = pmodel.getItem(idx);
            pmodel.removeItem(idx);
            preset_db.remove(p);
            pfmodel.invalidate();
        }
        break;
    case Util::Kitpiece:
        // we need to go from the end, otherwise it'll end in disaster
        for (int s_idx = idxs.count() - 1; s_idx >= 0; s_idx--) {
            int idx = idxs[s_idx];
            QSharedPointer<Kitpiece> kp = kpmodel.getItem(idx);
            kpmodel.removeItem(idx);
            kitpiece_db.remove(kp);
            kpfmodel.invalidate();
        }
        break;
    }
}

void BFD3ME::on_saveBtn_clicked()
{
    // bail out if no data
    if (!ui->itemlist->model())
        return;

    QList<int> idxs = getIdxList();

    switch (_type) {
    case Util::Kitpiece:
        {
            for (int i = 0; i < idxs.count(); i++) {
                int idx = idxs[i];
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
                kpfmodel.invalidate();
            }
        }
        break;
    case Util::Kit:
        {
            for (int i = 0; i < idxs.count(); i++) {
                int idx = idxs[i];
                QSharedPointer<Kit> k = kmodel.getItem(idx);
                if (ui->nameCheck->isChecked())
                    k->setName(ui->nameEdit->text());
                if (ui->libcodeCheck->isChecked())
                    k->setLibcode(ui->libcodeEdit->text());
                if (ui->libnameCheck->isChecked())
                    k->setLibname(ui->libnameEdit->text());
                kfmodel.invalidate();
            }
        }
        break;
    case Util::Preset:
        {
            for (int i = 0; i < idxs.count(); i++) {
                int idx = idxs[i];
                QSharedPointer<Preset> p = pmodel.getItem(idx);
                if (ui->nameCheck->isChecked())
                    p->setName(ui->nameEdit->text());
                if (ui->libcodeCheck->isChecked())
                    p->setLibcode(ui->libcodeEdit->text());
                if (ui->libnameCheck->isChecked())
                    p->setLibname(ui->libnameEdit->text());
                pfmodel.invalidate();
            }
        }
        break;
    }

    // if database, we're saving everything at once
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
    // otherwise, save each selected file
    } else {
        switch (_type) {
        case Util::Kit:
            for (int i = 0; i < idxs.count(); i++) {
                int idx = idxs[i];
                QSharedPointer<Kit> k = kmodel.getItem(idx);
                kit_f.save(k);
            }
            kfmodel.invalidate();
            break;
        case Util::Preset:
            for (int i = 0; i < idxs.count(); i++) {
                int idx = idxs[i];
                QSharedPointer<Preset> p = pmodel.getItem(idx);
                preset_f.save(p);
            }
            pfmodel.invalidate();
            break;
        case Util::Kitpiece:
            for (int i = 0; i < idxs.count(); i++) {
                int idx = idxs[i];
                QSharedPointer<Kitpiece> kp = kpmodel.getItem(idx);
                kitpiece_f.save(kp);
            }
            kpfmodel.invalidate();
            break;
        }
    }
    ui->itemlist->selectionModel()->clearSelection();
}
