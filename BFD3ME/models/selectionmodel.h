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

