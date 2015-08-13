#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QAbstractItemModel>
#include <QSharedPointer>
#include <QList>

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
    int total_size;
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
        return createIndex(row, col, (void*) &p_list[p.row()]);
    }

    QModelIndex parent(const QModelIndex &child) const {
        if (child.internalPointer() != 0) {
            tree_data *ptr = (tree_data*) child.internalPointer();
            createIndex(ptr->row_id, 0);
        }
        return QModelIndex();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const {
        if (role == Qt::DisplayRole) {
            if (index.internalPointer() == 0)
                return p_list.at(index.row()).str;
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
        total_size = count;

        // sort the library names
        QStringList parent_list = child_map.keys();
        qSort(parent_list);
        for (int i = 0; i < parent_list.count(); i++) {
            tree_data d = {i, parent_list[i] };
            p_list.append(d);
        }
        emit dataChanged(createIndex(0, 0), createIndex(count, 0));
    }

    void setItem(QSharedPointer<T> newItem, QModelIndex index) {
        tree_data *ptr = (tree_data*) index.internalPointer();
        QList<QSharedPointer<T>> &list = child_map[ptr->str];
        QSharedPointer<T> oldItem = list.at(index.row());
        *oldItem = *newItem;

        emit dataChanged(index, index);
    }

    void removeItem(QModelIndex index) {
        tree_data *ptr = (tree_data*) index.internalPointer();
        QList<QSharedPointer<T>> &list = child_map[ptr->str];
        int idx = index.row();
        list.removeAt(idx);
        total_size--;
        emit dataChanged(index, index);
    }

    void clearList() {
        child_map.clear();
        p_list.clear();
        emit dataChanged(createIndex(0, 0), createIndex(total_size, 0));
    }
};

#endif // KITMODEL_H
