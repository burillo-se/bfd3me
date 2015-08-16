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

#ifndef BFD3ME_H
#define BFD3ME_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QLineEdit>
#include <QThread>
#include <QObject>
#include <QSettings>

#include "utils/util.h"
#include "items/kit.h"
#include "items/kitpiece.h"
#include "items/preset.h"
#include "helpers/helper.h"
#include "helpers/dbhelper.h"
#include "models/itemmodel.h"
#include "models/filtermodel.h"
#include "models/selectionmodel.h"

namespace Ui {
class BFD3ME;
}

class BFD3ME : public QMainWindow
{
    Q_OBJECT

public:
    explicit BFD3ME(QWidget *parent = 0);
    ~BFD3ME();

private slots:
    void on_mode_f_toggled(bool checked);
    void on_mode_db_toggled(bool checked);
    void on_type_kitpiece_toggled(bool checked);
    void on_type_kit_toggled(bool checked);
    void on_type_preset_toggled(bool checked);
    void on_comboBox_currentIndexChanged(int index);
    void selected();
    void on_browseBtn_clicked();
    void on_deleteBtn_clicked();
    void save();
    void restore();
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();
    void load();
    void progressChanged(QString progressStr, int progressDone, int progressTodo);
    void error(QString path, QString errorStr);
    void finished();

    void on_pathEdit_currentTextChanged(const QString &arg1);

    void on_lineEdit_textChanged(const QString &arg1);

private:
    void setMode(Util::Mode mode);
    void setType(Util::Type type);
    void setPathEditItems();
    QStringList getPastPaths();
    void setNewPastPath();
    void clearAll();
    void clearData();
    void displayEdits();
    void setFilterStrings();
    void setEnabledButtons();
    void setText(const QString &text, QLineEdit *l, bool first);
    Util::Mode _mode;
    Util::Type _type;
    Ui::BFD3ME *ui;

    Helper<Kit> kit_f;
    Helper<Kitpiece> kitpiece_f;
    Helper<Preset> preset_f;
    DBHelper<Kit> kit_db;
    DBHelper<Kitpiece> kitpiece_db;
    DBHelper<Preset> preset_db;

    ItemModel<Kit> kmodel;
    ItemModel<Kitpiece> kpmodel;
    ItemModel<Preset> pmodel;

    FilterModel<Kit> kfmodel;
    FilterModel<Kitpiece> kpfmodel;
    FilterModel<Preset> pfmodel;

    SelectionModel<Kit> kselection;
    SelectionModel<Kitpiece> kpselection;
    SelectionModel<Preset> pselection;

    QThread loadThread;
    QThread saveThread;
    QThread restoreThread;
    QStringList errors;
    QSettings settings;
};

#endif // BFD3ME_H
