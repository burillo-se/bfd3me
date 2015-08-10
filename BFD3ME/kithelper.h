#ifndef KITHELPER_H
#define KITHELPER_H

#include "kit.h"

#include <QList>
#include <QMap>
#include <QSharedPointer>

class KitHelper
{
private:
    struct kit_info {
        QDomDocument doc;
        QString path;
    };
    QMap<QSharedPointer<Kit>, kit_info> info_map;
public:
    void saveKit(QSharedPointer<Kit> &k);
    QSharedPointer<Kit> restoreKitFromBackup(QSharedPointer<Kit> &k);
    static QList<QSharedPointer<Kit> > loadKits(QString &path);
};

#endif // KITHELPER_H
