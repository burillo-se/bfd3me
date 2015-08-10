#include "kithelper.h"
#include "util.h"

#include <QFile>
#include <QDir>

static QSharedPointer<Kit> loadKit(QDomDocument &doc) {
    QDomElement node = doc.firstChildElement("BFD2KitInfo");
    return QSharedPointer<Kit>(new Kit(node));
}

static QDomDocument loadDoc(QString &path) {
    QDomDocument doc;
    QFile f(path);
    f.open(QFile::ReadOnly);

    QTextStream in(&f);
    doc.setContent(in.readAll());
    f.close();

    return doc;
}

void KitHelper::saveKit(QSharedPointer<Kit> &k) {
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

QSharedPointer<Kit> KitHelper::restoreKitFromBackup(QSharedPointer<Kit> &k) {
    QString &path = info_map[k].path;
    QString bkp_path = Util::getLastBackupPath(path);
    QDomDocument &doc = info_map[k].doc;

    // replace with backup
    QFile::remove(path);
    QFile::rename(bkp_path, path);

    // reload the doc
    QDomDocument new_doc = loadDoc(path);

    // update our map
    QSharedPointer<Kit> new_k = loadKit(new_doc);
    kit_info new_i = {new_doc, path};
    info_map.remove(k);
    info_map.insert(new_k, new_i);
    return new_k;
}

QList<QSharedPointer<Kit> > KitHelper::loadKits(QString &path) {
    QList<QSharedPointer<Kit>> result;
    QDir d(path);
    QStringList filter;
    filter << "*.bfd3kit";
    foreach (QFileInfo fi, d.entryInfoList(filter, QDir::Files)) {
        QDomDocument doc = loadDoc(fi.absoluteFilePath());
        QSharedPointer<Kit> k = loadKit(doc);
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
