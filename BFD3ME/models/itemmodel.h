#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QAbstractItemModel>
#include <QSharedPointer>
#include <QList>
#include <QMutex>

#include <QtDebug>

/*
 * This model is a horrible contraption that basically takes a
 * QList<> of items and creates a tree out of it. Since it's not
 * a real tree-like structure, we need lots of supporting
 * infrastructure to fake our way into a tree-like existence.
 */

template <typename T>
class ItemModel : public QAbstractItemModel
{
private:
    struct tree_data {
        int row_id;
        QString str;
    };

    QList<tree_data> p_list;
    QMap<QString,QList<QSharedPointer<T>>> child_map;
    QMutex mutex;
public:
    int rowCount(const QModelIndex &index) const {
        if (index == QModelIndex())
            return p_list.count();
        if (index.internalPointer() != 0)
            return 0;
        int parent_idx = index.row();
        QString lib = p_list[parent_idx].str;
        return child_map[lib].count();
    }

    int columnCount(const QModelIndex &) const { return 1;}

    QModelIndex index(int row, int col, const QModelIndex &p) const {
        if (!hasIndex(row, col, p))
            return QModelIndex();

        // this is root node
        if (!p.isValid())
            return createIndex(row, col);

        // this is third-level node, which means it can't exist
        if (p.internalPointer() != 0) {
            return QModelIndex();
        }
        QModelIndex result = createIndex(row, col, (void*) &p_list[p.row()]);
        return result;
    }

    QModelIndex parent(const QModelIndex &child) const {
        if (child.internalPointer() != 0) {
            tree_data *ptr = (tree_data*) child.internalPointer();
            return createIndex(ptr->row_id, 0);
        }
        return QModelIndex();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        if (role == Qt::DisplayRole) {
            if (index.internalPointer() == 0) {
                if (p_list.at(index.row()).str == "")
                    return "<Unknown>";
                return p_list.at(index.row()).str;
            }
            tree_data *ptr = (tree_data*) index.internalPointer();
            return child_map[ptr->str].at(index.row())->getName();
        }
        return QVariant();
    }

    QSharedPointer<T> getItem(QModelIndex index) {
        tree_data *ptr = (tree_data*) index.internalPointer();
        QList<QSharedPointer<T>> &list = child_map[ptr->str];
        int idx = index.row();
        if (list.empty() || idx >= list.count())
            return QSharedPointer<T>();
        return list[idx];
    }

    void setList(QList<QSharedPointer<T>> &newList) {
        child_map.clear();
        p_list.clear();

        int count = newList.count();
        for (int i = 0; i < newList.count(); i++) {
            child_map[newList[i]->getLibname()].append(newList[i]);
        }

        // sort the library names
        QStringList parent_list = child_map.keys();
        qSort(parent_list);
        for (int i = 0; i < parent_list.count(); i++) {
            tree_data d = {i, parent_list[i] };
            p_list.append(d);
        }
        emit dataChanged(createIndex(0, 0), createIndex(count, 0));
    }

    void setItem(QSharedPointer<T> newItem, QModelIndex oldIndex) {
        tree_data *ptr = (tree_data*) oldIndex.internalPointer();
        QList<QSharedPointer<T>> &oldList = child_map[ptr->str];

        QString oldname = ptr->str;
        QString newname = newItem->getLibname();

        if (oldname != newname) {
            QModelIndex newIndex;
            tree_data *new_ptr;
            // if we don't have such a node, create it
            if (!child_map.keys().contains(newname)) {
                beginInsertRows(QModelIndex(), p_list.count(), p_list.count());
                tree_data new_data = {p_list.count(), newname};
                p_list.append(new_data);
                new_ptr = &p_list.last();
                endInsertRows();
            } else {
                // find row_id of the new node
                foreach (const tree_data &d, p_list) {
                    if (d.str == newname) {
                        new_ptr = (tree_data*) &d;
                        break;
                    }
                }
            }
            newIndex = index(new_ptr->row_id, 0, QModelIndex());

            // remove item from old list
            QList<QSharedPointer<T>> &newList = child_map[newname];
            beginRemoveRows(oldIndex, oldIndex.row(), oldIndex.row());
            oldList.removeAt(oldIndex.row());
            endRemoveRows();
            emit dataChanged(oldIndex, oldIndex);

            // add item to new list
            beginInsertRows(newIndex, newList.count(), newList.count());
            newList.append(newItem);
            endInsertRows();
            emit dataChanged(newIndex, newIndex);
        } else {
            // replace old item with new item
            QSharedPointer<T> oldItem = oldList.at(oldIndex.row());
            *oldItem = *newItem;
            emit dataChanged(oldIndex, oldIndex);
        }
    }

    void removeItem(QModelIndex parent) {
        tree_data *ptr = (tree_data*) parent.internalPointer();
        QList<QSharedPointer<T>> &list = child_map[ptr->str];
        int idx = parent.row();
        beginRemoveRows(parent, idx, idx);
        list.removeAt(idx);
        endRemoveRows();
        emit dataChanged(parent, parent);
    }

    void clearList() {
        int count = p_list.count();
        beginRemoveRows(QModelIndex(), 0, count);
        child_map.clear();
        p_list.clear();
        endRemoveRows();
        emit dataChanged(createIndex(0, 0), createIndex(count, 0));
    }
};

#endif // KITMODEL_H
