#ifndef HELPER_H
#define HELPER_H

#include<QString>
#include <QList>
#include <QMap>
#include <QSharedPointer>
#include <QDomDocument>

template<typename T>
class Helper
{
private:
    QString _tag;
    QString _filter;
    struct data_info {
        QDomDocument doc;
        QString path;
    };
    QMap<QSharedPointer<T>, data_info> _info_map;
public:
    Helper(const QString &tag, const QString &filter);
    void save(QSharedPointer<T> &k);
    QSharedPointer<T> restoreFromBackup(QSharedPointer<T> &k);
    QList<QSharedPointer<T> > load(const QString &path);
};

#endif // HELPER_H
