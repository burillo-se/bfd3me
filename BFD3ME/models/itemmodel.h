#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QAbstractItemModel>
#include <QSharedPointer>
#include <QList>

#include <QtDebug>

template <typename T>
class ItemModel : public QAbstractItemModel
{
private:
    QList<QSharedPointer<T>> list;
public:
    int rowCount(const QModelIndex &) const { return list.count();}
    int columnCount(const QModelIndex &) const { return 1;}
    QModelIndex index(int row, int column, const QModelIndex &) const {
        return createIndex(row, column);
    }
    QModelIndex parent(const QModelIndex &) const {
        return QModelIndex();
    }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        if (role == Qt::DisplayRole) {
            return list.at(index.row())->getName();
        }
        return QVariant();
    }
    QSharedPointer<T> getItem(int idx) {
        if (list.empty() || idx >= list.count())
            return QSharedPointer<T>();
        return list[idx];
    }
    void setList(QList<QSharedPointer<T>> &newList) {
        list.clear();
        list = newList;
        int count = list.count();
        emit dataChanged(createIndex(0, 0), createIndex(count, 0));
    }
    void setItem(QSharedPointer<T> newItem, int idx) {
        list[idx] = newItem;
        emit dataChanged(createIndex(idx, 0), createIndex(idx, 0));
    }
    void removeItem(int idx) {
        list.removeAt(idx);
        emit dataChanged(createIndex(idx - 1, 0), createIndex(idx, 0));
    }

    void clearList() {
        int count = list.count();
        list.clear();
        emit dataChanged(createIndex(0, 0), createIndex(count, 0));
    }
};

#endif // KITMODEL_H
