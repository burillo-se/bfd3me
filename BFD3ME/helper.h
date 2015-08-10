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
    Helper<T>(const QString &tag, const QString &filter);
    void save(QSharedPointer<T> &k);
    QSharedPointer<T> restoreFromBackup(QSharedPointer<T> &k);
    QList<QSharedPointer<T> > load(const QString &path);
};

template <typename T>
Helper<T>::Helper(const QString &tag, const QString &filter) {
    _tag = tag;
    _filter = filter;
}

template <typename T>
static QSharedPointer<T> alloc(QDomDocument &doc, const QString &tag) {
    QDomElement node = doc.firstChildElement(tag);
    return QSharedPointer<T>(new T(node));
}

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
void Helper<T>::save(QSharedPointer<T> &k) {
    QString &path = info_map[k].path;
    QString tmp_path = path + ".tmp";
    QString bkp_path = Util::getNewBackupPath(path);

    QDomDocument &doc = info_map[k].doc;

    QFile f(tmp_path);
    f.open(QFile::WriteOnly);
    QTextStream out( &file );
    doc.save(out, 4);
    f.close();

    // backup the original
    QFile::rename(path, bkp_path);
    QFile::rename(tmp_path, path);
}

template <typename T>
QSharedPointer<T> Helper<T>::restoreFromBackup(QSharedPointer<T> &k) {
    QString &path = info_map[k].path;
    QString bkp_path = Util::getLastBackupPath(path);
    QDomDocument &doc = info_map[k].doc;

    // replace with backup
    QFile::remove(path);
    QFile::rename(bkp_path, path);

    // reload the doc
    QDomDocument new_doc = loadDoc(path);

    // update our map
    QSharedPointer<T> new_k = load(new_doc);
    kit_info new_i = {new_doc, path};
    info_map.remove(k);
    info_map.insert(new_k, new_i);
    return new_k;
}

template <typename T>
QList<QSharedPointer<T> > Helper<T>::load(const QString &path) {
    QList<QSharedPointer<T>> result;
    QDir d(path);
    QStringList filter;
    filter << _filter;
    foreach (QFileInfo fi, d.entryInfoList(filter, QDir::Files)) {
        QDomDocument doc = loadDoc(fi.absoluteFilePath());
        QSharedPointer<T> k = alloc(doc, _tag);
        kit_info i = {doc, fi.absoluteFilePath()};
        info_map[k] = i;
        result.append(k);
    }
    // recurse into subdirectories
    foreach (QFileInfo fi, d.entryInfoList(QDir::Dirs)) {
        result.append(loadKits(fi.absoluteFilePath()));
    }
    return result;
}


#endif // HELPER_H
