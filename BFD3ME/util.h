#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QDomElement>

class Util
{
private:
public:
    Util();
    static void setNodeAttr(QDomElement &node,
                            const QString &attr_name,
                            const QString &attr_val);
    static QString getNewBackupPath(QString &path);
    static QString getLastBackupPath(QString &path);
    static bool matchesFilter(const QString &val,
                              const QString &filter);
};

#endif // UTIL_H
