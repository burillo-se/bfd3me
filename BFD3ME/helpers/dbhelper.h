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

#ifndef DBHELPER_H
#define DBHELPER_H

/*
 * This class is a wrapper around a kit/kitpiece/presets (item) database.
 *
 * Basically, a "database" is a giant XML file with all the items inside it.
 * They have the same structure as those items in separate files, so the
 * data is apparently just copied into the database verbatim. Which is why we are
 * able to parse the items with the same code we use to parse single files.
 *
 * The parsing goes the same way as in parsing regular files, except we don't recurse
 * into subdirectories (that would make no sense...) and we get the DOM nodes from
 * single file instead of from multiple files.
 */

#include <QList>
#include <QSharedPointer>

#include "helpers/helperbase.h"
#include "utils/util.h"

/*
 * Template class header
 *
 * Implementations are below.
 */
template <typename T>
class DBHelper : public HelperBase
{
private:
    QString _tag;
    QString _path;
    QDomDocument _doc;
    QMap<QSharedPointer<T>,QDomElement> _info_map;
public:
    DBHelper(const QString &tag);
    QList<QSharedPointer<T>> load(const QString &path);
    void save();
    QList<QSharedPointer<T>> restoreFromBackup();
    void remove(QSharedPointer<T> item);
    void clear();
};

template <typename T>
DBHelper<T>::DBHelper(const QString &tag) : HelperBase()
{
    _tag = tag;
}

template <typename T>
void DBHelper<T>::clear()
{
    _info_map.clear();
}

/*
 * Load a list of items given a path to the database file
 */
template <typename T>
QList<QSharedPointer<T>> DBHelper<T>::load(const QString &path) {
    QList<QSharedPointer<T>> result;

    clear();

    _path = path;
    _doc = loadDoc(path);

    if (_doc.isNull()) {
        return result;
    }

    QDomNodeList nodeList = _doc.elementsByTagName(_tag);
    _progressDone = 0;
    _progressTodo = nodeList.count();

    for (int i = 0; i < nodeList.count(); i++) {
        QDomElement el = nodeList.at(i).toElement();
        QSharedPointer<T> k(new T(el));
        result.append(k);
        _info_map.insert(k, el);
        _progressDone++;
        emit progressChanged("Parsing", _progressDone, _progressTodo);
    }
    emit finished();
    return result;
}

/*
 * Save an XML document to a file (making backup of the original
 */
template <typename T>
void DBHelper<T>::save() {
    saveDoc(_doc, _path);
}

/*
 * Restore older backup and reload the item list from that backup.
 */
template <typename T>
QList<QSharedPointer<T>> DBHelper<T>::restoreFromBackup() {
    restoreFile(_path);

    return load(_path);
}

/*
 * Remove a single item from the database.
 *
 * What happens is, since our database is a giant XML file and
 * we already parsed it into a QDomDocument, every item we have
 * is a QDomElement from that QDomDocument.
 *
 * Meaning, if we remove a QDomElement (going to its parent and
 * removing itself from the parent's child list), we effectively
 * remove an item from the QDomDocument, and it won't be present
 * the next time we dump it to disk.
 */
template <typename T>
void DBHelper<T>::remove(QSharedPointer<T> item) {
    QDomElement el = _info_map[item];
    el.parentNode().removeChild(el);
}

#endif // DBHELPER_H
