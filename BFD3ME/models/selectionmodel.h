#ifndef SELECTIONMODEL_H
#define SELECTIONMODEL_H

#include <QItemSelectionModel>

#include <QtDebug>

class SelectionModel : public QItemSelectionModel {
public slots:
    void select(const QItemSelection &selection, SelectionFlags command) {
        foreach (const QItemSelectionRange &range, selection) {
            for (int i = range.indexes().count() - 1; i >= 0; i--) {
                QModelIndex index = range.indexes().at(i);
                QModelIndex mi = model()->index(index.row(), index.column());
                qDebug() << index.internalPointer() << mi.internalPointer();
                if (index.internalPointer() != 0)
                    return;
            }
        }
        return;
        QItemSelectionModel::select(selection, command);
    }
};

#endif // SELECTIONMODEL_H

