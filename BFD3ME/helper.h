#ifndef HELPER_H
#define HELPER_H

/*
 * This class is a wrapper around a list of XML files representing Items.
 *
 * Each Item sits in a an XML file of its own.
 *
 * The parsing goes as follows:
 * 1) open and parse the XML file with Qt XML
 * 2) find the tag we're looking for
 * 3) construct an object given the tag
 * 5) recurse into subdirs
 * 4) rinse, repeat until we run out of files to parse
 *
 * This can take long, so the program will actually run the "load" function
 * in a separate thread.
 */

#include <QString>
#include <QList>
#include <QMap>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QSharedPointer>
#include <QDomDocument>
#include <QQueue>

/*
 * Header
 */
template<typename T>
class Helper
{
private:
    QString _tag;
    QString _filter;
    /*
     * we want to store some data to make sure we can save the kits back to
     * where they came from where the time comes.
     */
    struct data_info {
        QDomDocument doc;
        QString path;
    };
    QMap<QSharedPointer<T>, data_info> _info_map;
    /*
     * This is for informational purposes only.
     * TODO: include error messages
     */
    QStringList errors;
    QSharedPointer<T> loadOne(const QString &path);
public:
    Helper<T>(const QString &tag, const QString &filter);
    void save(QSharedPointer<T> k);
    QSharedPointer<T> restoreFromBackup(QSharedPointer<T> k);
    QList<QSharedPointer<T> > load(const QString &path);
    QStringList getErrors() const;
};

template <typename T>
Helper<T>::Helper(const QString &tag, const QString &filter) {
    _tag = tag;
    _filter = filter;
}

/*
 * Item allocation, given a QDomDocument
 */
template <typename T>
static QSharedPointer<T> alloc(QDomDocument &doc, const QString &tag) {
    QDomNodeList nodes = doc.elementsByTagName(tag);
    // if we didn't find anything, or found strange metadata,
    // we can't do anything
    if (nodes.count() != 1) {
        return QSharedPointer<T>();
    }
    QDomElement el = nodes.at(0).toElement();
    return QSharedPointer<T>(new T(el));
}

/*
 * Get a QDomDocument given a file path
 */
static QDomDocument loadDoc(const QString &path) {
    QDomDocument doc;
    QFile f(path);
    f.open(QFile::ReadOnly);

    QTextStream in(&f);
    doc.setContent(in.readAll());
    f.close();

    return doc;
}

template <typename T>
QStringList Helper<T>::getErrors() const {
    return errors;
}

/*
 * Save a given item into a path we know it resides in
 */
template <typename T>
void Helper<T>::save(QSharedPointer<T> k) {
    QString &path = _info_map[k].path;
    QString tmp_path = path + ".tmp";
    QString bkp_path = Util::getNewBackupPath(path);

    QDomDocument &doc = _info_map[k].doc;

    QFile f(tmp_path);
    f.open(QFile::WriteOnly);
    QTextStream out( &f );
    doc.save(out, 4);
    f.close();

    // backup the original
    QFile::rename(path, bkp_path);
    QFile::rename(tmp_path, path);
}

/*
 * Restore an item from latest backup and reload it
 */
template <typename T>
QSharedPointer<T> Helper<T>::restoreFromBackup(QSharedPointer<T> k) {
    QString &path = _info_map[k].path;
    QString bkp_path = Util::getLastBackupPath(path);

    // replace with backup
    if (bkp_path != path) {
        QFile::remove(path);
        QFile::rename(bkp_path, path);
    }

    QSharedPointer<T> new_k = loadOne(path);

    // update our map
    _info_map.remove(k);

    return new_k;
}

template <typename T>
QSharedPointer<T> Helper<T>::loadOne(const QString &path) {
    QDomDocument doc = loadDoc(path);
    QSharedPointer<T> new_k = alloc<T>(doc, _tag);
    data_info new_i = {doc, path};
    _info_map.insert(new_k, new_i);

    return new_k;
}

/*
 * Load a list of items given a path
 *
 * This also recurses into subdirs.
 */
template <typename T>
QList<QSharedPointer<T> > Helper<T>::load(const QString &path) {
    QList<QSharedPointer<T>> result;
    QStringList filter;
    filter << _filter;

    // clear the list of errors
    errors.clear();

    // create a FIFO for our paths to examine next
    QQueue<QString> path_fifo;
    path_fifo.push_back(path);

    while(!path_fifo.isEmpty()) {
        QDir d(path_fifo.front());
        path_fifo.pop_front();
        foreach (QFileInfo fi, d.entryInfoList(filter, QDir::Files)) {
            QDomDocument doc = loadDoc(fi.absoluteFilePath());
            QSharedPointer<T> k = alloc<T>(doc, _tag);

            // if we found a strange XML document that we couldn't parse
            if (k.isNull()) {
                errors << fi.absoluteFilePath();
                continue;
            }

            data_info i = {doc, fi.absoluteFilePath()};
            _info_map[k] = i;
            result.append(k);
        }
        // recurse into subdirectories
        foreach (QFileInfo fi, d.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            path_fifo.push_back(fi.absoluteFilePath());
        }
    }

    return result;
}


#endif // HELPER_H
