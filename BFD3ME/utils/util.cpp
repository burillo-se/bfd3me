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

#include "util.h"

#include <QFile>
#include <QFileInfo>

/*
 * Since empty string is always contained within any other string,
 * we need a way to filter out stuff that has an explicitly empty
 * attribute (such as a Kitpiece without a subclass).
 *
 * So, here we test for contains() if the string isn't empty, but
 * "match" the empty filter string only if the value is also an
 * empty string.
 */
bool Util::matchesFilter(const QString &val, const QString &filter) {
    if (val != "" && filter.toLower().contains(val.toLower()))
        return true;
    if (val == "" && filter == "")
        return true;
    return false;
}

/*
 * We don't want to have empty node attributes, so we remove them
 * if we are supplied an empty value.
 */
void Util::setNodeAttr(QDomElement &node, const QString &attr_name,
                       const QString &attr_val) {
    if (attr_val != "") {
        node.setAttribute(attr_name, attr_val);
    } else {
        node.removeAttribute(attr_name);
    }
}

/*
 * Backup paths go as follows:
 * file.xml.bak
 * file.xml.bak.0
 * file.xml.bak.1
 * ...
 *
 * Here, we get the last *existing* backup path.
 */
QString Util::getLastBackupPath(const QString &path) {
    QString result;
    if (QFileInfo::exists(path + ".bak")) {
        result = path + ".bak";
        unsigned num = 0;
        while (QFileInfo::exists(QString("%0.%1").arg(result).arg(num))) {
               num++;
        }
        if (num != 0)
            result += QString(".%0").arg(num - 1);
    }
    return result != QString() ? result : path;
}

/*
 * Get the first *non-existing* backup path
 */
QString Util::getNewBackupPath(const QString &path) {
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
