#include "helper.h"
#include "util.h"

#include <QFile>
#include <QDir>
#include <QDomElement>

Helper::Helper(const QString &tag, const QString &filter) {
    _tag = tag;
    _filter = filter;
}

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

void Helper::save(QSharedPointer<T> &k) {
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

QSharedPointer<T> Helper::restoreFromBackup(QSharedPointer<T> &k) {
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

QList<QSharedPointer<T> > Helper::load(const QString &path) {
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
