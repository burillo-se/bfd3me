#include "bfd3me.h"
#include "ui_bfd3me.h"

#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

#include "utils/util.h"

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
#define LOAD(x, y) \
    if (_mode == Util::Database) {\
        ##x##model.setList(##y##_db.load(ui->pathEdit->text())); \
    } else {\
        ##x##model.setList(##y##_f.load(ui->pathEdit->text())); \
    } \
    ##x##fmodel.setSourceModel(&##x##model); \
    ##x##selection.setModel(&##x##fmodel); \
    ui->itemlist->setModel(&##x##fmodel); \
    ui->itemlist->setSelectionModel(&##x##selection); \
    connect(&##x##selection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed())); \
    ##x##fmodel.invalidate();

void BFD3ME::load() {
    disconnect(&kselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    disconnect(&kpselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    disconnect(&pselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    errors.clear();
    switch (_type) {
    case Util::Kit:
        LOAD(k, kit)
        break;
    case Util::Kitpiece:
        LOAD(kp, kitpiece)
        break;
    case Util::Preset:
        LOAD(p, preset)
        break;
    }
}

#define RESTORE1(x,y) \
    do { \
    ##x##model.setList(##y##_db.restoreFromBackup()); \
    ##x##fmodel.invalidate(); \
    ##x##selection.clearSelection(); \
    } while (0);

#define RESTORE2(x,y,z) \
    idxs = ##x##selection.selectedIndexes(); \
    for (int i = 0; i < idxs.count(); i++) { \
        QModelIndex idx = ##x##fmodel.mapToSource(idxs[i]); \
        QSharedPointer<z> (x) = ##x##model.getItem(idx); \
        ##x##model.setItem(##y##_f.restoreFromBackup((x)), idx); \
    } \
    ##x##fmodel.invalidate(); \
    ##x##selection.clearSelection();

void BFD3ME::on_restoreBtn_clicked()
{
    // bail out if no data
    if (!ui->itemlist->model())
        return;

    disconnect(&kselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    disconnect(&kpselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    disconnect(&pselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));

    QModelIndexList idxs;

    // if database, we're restoring everything at once
    if (_mode == Util::Database) {
        switch (_type) {
        case Util::Kit:
            RESTORE1(k, kit)
            break;
        case Util::Kitpiece:
            RESTORE1(kp, kitpiece)
            break;
        case Util::Preset:
            RESTORE1(p, preset)
            break;
        }
    // otherwise, restore each selected file
    } else {
        switch (_type) {
        case Util::Kit:
            RESTORE2(k, kit, Kit)
            break;
        case Util::Preset:
            RESTORE2(p, preset, Preset)
            break;
        case Util::Kitpiece:
            RESTORE2(kp, kitpiece, Kitpiece)
            break;
        }
    }
}

// we need to go from the end, otherwise it'll end in disaster
#define DELETE(x, y, z) \
    idxs = ##x##selection.selectedIndexes(); \
    for (int s_idx = idxs.count() - 1; s_idx >= 0; s_idx--) { \
        QModelIndex idx = ##x##fmodel.mapToSource(idxs[s_idx]); \
        QSharedPointer<z> (x) = ##x##model.getItem(idx); \
        ##x##model.removeItem(idx); \
        ##y##_db.remove((x)); \
    } \
    ##x##fmodel.invalidate(); \
    connect(&##x##selection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed())); \
    ##x##selection.clearSelection();

void BFD3ME::on_deleteBtn_clicked()
{
    // bail out if no data
    if (!ui->itemlist->model())
        return;

    QModelIndexList idxs;
    disconnect(&kselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    disconnect(&kpselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    disconnect(&pselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));

    // we already know we're in database mode, so check type only
    switch (_type) {
    case Util::Kit:
        DELETE(k, kit, Kit)
        break;
    case Util::Preset:
        DELETE(p, preset, Preset)
        break;
    case Util::Kitpiece:
        DELETE(kp, kitpiece, Kitpiece)
        break;
    }
}

void BFD3ME::on_saveBtn_clicked()
{
    // bail out if no data
    if (!ui->itemlist->model())
        return;

    QModelIndexList idxs;

    disconnect(&kselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    disconnect(&kpselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    disconnect(&pselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));

    switch (_type) {
    case Util::Kitpiece:
        {
            idxs = kpselection.selectedIndexes();
            for (int i = idxs.count() - 1; i >= 0; i--) {
                QModelIndex idx = kpfmodel.mapToSource(idxs[i]);
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
                if (_mode == Util::Files) {
                    kitpiece_f.save(kp);
                }
                kpmodel.setItem(kp, idx);
            }
            kpfmodel.invalidate();
            connect(&kselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
            kpselection.clearSelection();
        }
        break;
    case Util::Kit:
        {
            idxs = kselection.selectedIndexes();
            for (int i = idxs.count() - 1; i >= 0; i--) {
                QModelIndex idx = kfmodel.mapToSource(idxs[i]);
                QSharedPointer<Kit> k = kmodel.getItem(idx);
                if (ui->nameCheck->isChecked())
                    k->setName(ui->nameEdit->text());
                if (ui->libcodeCheck->isChecked())
                    k->setLibcode(ui->libcodeEdit->text());
                if (ui->libnameCheck->isChecked())
                    k->setLibname(ui->libnameEdit->text());
                if (_mode == Util::Files) {
                    kit_f.save(k);
                }
                kmodel.setItem(k, idx);
            }
            kfmodel.invalidate();
            connect(&kpselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
            kselection.clearSelection();
        }
        break;
    case Util::Preset:
        {
            idxs = pselection.selectedIndexes();
            for (int i = idxs.count() - 1; i >= 0; i--) {
                QModelIndex idx = pfmodel.mapToSource(idxs[i]);
                QSharedPointer<Preset> p = pmodel.getItem(idx);
                if (ui->nameCheck->isChecked())
                    p->setName(ui->nameEdit->text());
                if (ui->libcodeCheck->isChecked())
                    p->setLibcode(ui->libcodeEdit->text());
                if (ui->libnameCheck->isChecked())
                    p->setLibname(ui->libnameEdit->text());
                if (_mode == Util::Files) {
                    preset_f.save(p);
                }
                pmodel.setItem(p, idx);
            }
            pfmodel.invalidate();
            connect(&pselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
            pselection.clearSelection();
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
    }
}
