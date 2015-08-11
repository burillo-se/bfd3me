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

#include <QString>
#include <QDomDocument>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QSharedPointer>

/*
 * Template class header
 *
 * Implementations are below.
 */
template <typename T>
class DBHelper
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
};

template <typename T>
DBHelper<T>::DBHelper(const QString &tag)
{
    _tag = tag;
}

/*
 * Load a list of items given a path to the database file
 */
template <typename T>
QList<QSharedPointer<T>> DBHelper<T>::load(const QString &path) {
    _path = path;
    QFile f(_path);
    f.open(QFile::ReadOnly);
    QTextStream in(&f);
    _doc.setContent(in.readAll());
    f.close();

    QList<QSharedPointer<T>> result;
    QDomNodeList nodeList = _doc.elementsByTagName(_tag);
    for (int i = 0; i < nodeList.count(); i++) {
        QDomElement el = nodeList.at(i).toElement();
        QSharedPointer<T> k(new T(el));
        result.append(k);
        _info_map.insert(k, el);
    }
    return result;
}

/*
 * Save an XML document to a file (making backup of the original
 */
template <typename T>
void DBHelper<T>::save() {
    QString tmp_path = _path + ".tmp";
    QString bkp_path = Util::getNewBackupPath(_path);

    QFile f(tmp_path);
    f.open(QFile::WriteOnly);
    QTextStream out( &f );
    _doc.save(out, 4);
    f.close();

    // backup the original
    QFile::rename(_path, bkp_path);
    QFile::rename(tmp_path, _path);
}

/*
 * Restore older backup and reload the item list from that backup.
 */
template <typename T>
QList<QSharedPointer<T>> DBHelper<T>::restoreFromBackup() {
    QString bkp_path = Util::getLastBackupPath(_path);

    // replace with backup
    if (bkp_path != _path) {
        QFile::remove(_path);
        QFile::rename(bkp_path, _path);
    }

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
