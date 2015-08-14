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
    connect(ui->loadBtn, SIGNAL(pressed()), &loadThread, SLOT(start()));
    connect(&loadThread, SIGNAL(started()), this, SLOT(load()));
    connect(&loadThread, SIGNAL(finished()), ui->statusBar, SLOT(clearMessage()));

    connect(ui->saveBtn, SIGNAL(pressed()), &saveThread, SLOT(start()));
    connect(&saveThread, SIGNAL(started()), this, SLOT(save()));
    connect(&saveThread, SIGNAL(finished()), ui->statusBar, SLOT(clearMessage()));

    connect(ui->restoreBtn, SIGNAL(pressed()), &restoreThread, SLOT(start()));
    connect(&restoreThread, SIGNAL(started()), this, SLOT(restore()));
    connect(&restoreThread, SIGNAL(finished()), ui->statusBar, SLOT(clearMessage()));

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
    if (ui->pathEdit->text().isEmpty()) {
        loadThread.quit();
        return;
    }
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

#define RESTORE_DB(x,y) \
    do { \
    ##x##model.setList(##y##_db.restoreFromBackup()); \
    ##x##fmodel.invalidate(); \
    ##x##selection.clearSelection(); \
    } while (0);

#define RESTORE_F(x,y,z) \
    idxs = ##x##selection.selectedIndexes(); \
    for (int i = idxs.count() - 1; i >= 0; i--) { \
        progressChanged("Restoring", idxs.count() - i, idxs.count()); \
        QModelIndex idx = ##x##fmodel.mapToSource(idxs[i]); \
        QSharedPointer<z> (x) = ##x##model.getItem(idx); \
        ##x##model.setItem(##y##_f.restoreFromBackup((x)), idx); \
    } \
    ##x##fmodel.invalidate(); \
    ##x##selection.clearSelection();

void BFD3ME::restore()
{
    // bail out if no data
    if (!ui->itemlist->model()) {
        restoreThread.quit();
        return;
    }

    disconnect(&kselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    disconnect(&kpselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    disconnect(&pselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));

    QModelIndexList idxs;


    // if database, we're restoring everything at once
    if (_mode == Util::Database) {
        switch (_type) {
        case Util::Kit:
            RESTORE_DB(k, kit)
            break;
        case Util::Kitpiece:
            RESTORE_DB(kp, kitpiece)
            break;
        case Util::Preset:
            RESTORE_DB(p, preset)
            break;
        }
    // otherwise, restore each selected file
    } else {
        switch (_type) {
        case Util::Kit:
            RESTORE_F(k, kit, Kit)
            break;
        case Util::Preset:
            RESTORE_F(p, preset, Preset)
            break;
        case Util::Kitpiece:
            RESTORE_F(kp, kitpiece, Kitpiece)
            break;
        }
    }
    restoreThread.quit();
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

#define SAVE_PROPERTY(x, y, z) \
    if (ui->##y##Check->isChecked()) \
        (x)->set##z##(ui->##y##Edit->text());

void BFD3ME::save()
{
    // bail out if no data
    if (!ui->itemlist->model()) {
        saveThread.quit();
        return;
    }

    QModelIndexList idxs;

    disconnect(&kselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    disconnect(&kpselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));
    disconnect(&pselection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_selection_changed()));

    switch (_type) {
    case Util::Kitpiece:
        {
            idxs = kpselection.selectedIndexes();
            for (int i = idxs.count() - 1; i >= 0; i--) {
                progressChanged("Saving", idxs.count() - i, idxs.count());
                QModelIndex idx = kpfmodel.mapToSource(idxs[i]);
                QSharedPointer<Kitpiece> kp = kpmodel.getItem(idx);
                SAVE_PROPERTY(kp, name, Name);
                SAVE_PROPERTY(kp, libcode, Libcode);
                SAVE_PROPERTY(kp, libname, Libname);
                SAVE_PROPERTY(kp, class, Class);
                SAVE_PROPERTY(kp, subclass, Subclass);
                SAVE_PROPERTY(kp, manufacturer, Manufacturer);
                SAVE_PROPERTY(kp, model, Model);
                SAVE_PROPERTY(kp, date, Date);
                SAVE_PROPERTY(kp, dimensions, Dimensions);
                SAVE_PROPERTY(kp, beater, Beater);
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
                progressChanged("Saving", idxs.count() - i, idxs.count());
                QModelIndex idx = kfmodel.mapToSource(idxs[i]);
                QSharedPointer<Kit> k = kmodel.getItem(idx);
                SAVE_PROPERTY(k, name, Name);
                SAVE_PROPERTY(k, libcode, Libcode);
                SAVE_PROPERTY(k, libname, Libname);
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
                progressChanged("Saving", idxs.count() - i, idxs.count());
                QModelIndex idx = pfmodel.mapToSource(idxs[i]);
                QSharedPointer<Preset> p = pmodel.getItem(idx);
                SAVE_PROPERTY(p, name, Name);
                SAVE_PROPERTY(p, libcode, Libcode);
                SAVE_PROPERTY(p, libname, Libname);
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
    saveThread.quit();
}
