#ifndef SELECTIONMODEL_H
#define SELECTIONMODEL_H

#include <QItemSelectionModel>

#include "filtermodel.h"

/*
 * The selection model is coupled with filter model. Yay loose coupling!
 */

#include <QtDebug>

template <typename T>
class SelectionModel : public QItemSelectionModel {
public slots:
    void select(const QItemSelection &selection, SelectionFlags command) {
        foreach (const QItemSelectionRange &range, selection) {
            for (int i = range.indexes().count() - 1; i >= 0; i--) {
                QModelIndex index = range.indexes().at(i);
                FilterModel<T> *m = (FilterModel<T>*) model();
                QModelIndex srcIndex = m->mapToSource(index);
                if (srcIndex.internalPointer() == 0)
                    return;
            }
        }
        QItemSelectionModel::select(selection, command);
    }
};

#endif // SELECTIONMODEL_H

