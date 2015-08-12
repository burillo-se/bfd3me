#ifndef BFD3ME_H
#define BFD3ME_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QLineEdit>
#include <QThread>
#include <QObject>

#include "util.h"
#include "kit.h"
#include "kitpiece.h"
#include "preset.h"
#include "helper.h"
#include "dbhelper.h"
#include "itemmodel.h"

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
    void on_loadBtn_clicked();
    void on_selection_changed();
    void on_browseBtn_clicked();
    void on_restoreBtn_clicked();
    void on_deleteBtn_clicked();
    void on_saveBtn_clicked();
    void load();
    void progressChanged(QString progressStr, int progressDone, int progressTodo);

private:
    void setMode(Util::Mode mode);
    void setType(Util::Type type);
    void setDefaultDatabasePath();
    void clearAll();
    void displayEdits();
    void setFilterStrings();
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

    QSortFilterProxyModel fmodel;
    QThread loadThread;
};

#endif // BFD3ME_H
