#include "util.h"

#include <QFile>
#include <QFileInfo>

bool Util::matchesFilter(const QString &val, const QString &filter) {
    if (val != "" && filter.contains(val))
        return true;
    if (val == "" && filter == "")
        return true;
    return false;
}

void Util::setNodeAttr(QDomElement &node, const QString &attr_name,
                       const QString &attr_val) {
    if (attr_val != "") {
        node.setAttribute(attr_name, attr_val);
    } else {
        node.removeAttribute(attr_name);
    }
}

QString Util::getLastBackupPath(QString &path) {
    QString result;
    if (QFileInfo::exists(path + ".bak")) {
        result = path + ".bak";
        unsigned num = 0;
        while (QFileInfo::exists(QString("%0.%1").arg(result).arg(num))) {
               num++;
        }
        result += QString(".%0").arg(num - 1);
    }
    return result;
}

QString Util::getNewBackupPath(QString &path) {
    QString result = path + ".bak";
    if (QFileInfo::exists(result)) {
        unsigned num = 0;
        while (QFileInfo::exists(QString("%0.%1").arg(result).arg(num))) {
               num++;
        }
        result += QString(".%0").arg(num);
    }
    return result;
}
