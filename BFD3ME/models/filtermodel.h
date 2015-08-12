#ifndef FILTERMODEL_H
#define FILTERMODEL_H

/*
 * This code is full of fail and breaks lots of rules.
 */

#include <QSortFilterProxyModel>
#include "models/itemmodel.h"
#include "utils/util.h"
#include "items/kitpiece.h"

template <typename T>
class FilterModel : public QSortFilterProxyModel
{
private:
    Util::FilterType filter_type;
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &) const;
    bool filterAcceptsColumn(int , const QModelIndex &) const;
public:
    void setFilterType(Util::FilterType type) {filter_type = type;}
};

/*
 * This is a weird contraption.
 * Basically, all of the types we use in the program are unrelated,
 * but they share a "trait" (in boost-speak) in form of getFilteredString,
 * so that they can be filtered anyway.
 */
template <typename T>
bool FilterModel<T>::filterAcceptsRow(int source_row, const QModelIndex &) const {
    ItemModel<T>* model = (ItemModel<T>*) sourceModel();
    QSharedPointer<T> i = model->getItem(source_row);
    if (i.isNull())
        return false;
    i->setFilteredString(filter_type);
    if (filter_type == Util::No_Filter) {
        return true;
    } else {
        return Util::matchesFilter(filterRegExp().pattern(), i->getFilteredString());
    }
}

template <typename T>
bool FilterModel<T>::filterAcceptsColumn(int, const QModelIndex &) const {
    return true;
}


#endif // FILTERMODEL_H
