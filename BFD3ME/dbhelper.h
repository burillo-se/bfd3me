#ifndef DBHELPER_H
#define DBHELPER_H

#include <QString>
#include <QDomDocument>
#include <QList>
#include <QSharedPointer>

template <typename T>
class DBHelper
{
private:
    QString _tag;
    QString _path;
    QDomDocument _doc;
public:
    DBHelper(const QString &tag);
    QList<QSharedPointer<T>> load(const QString &path);
    void save();
    QList<QSharedPointer<T>> restoreFromBackup();
};

#endif // DBHELPER_H
