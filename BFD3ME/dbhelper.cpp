#include "dbhelper.h"

#include <QFile>

DBHelper::DBHelper(const QString &tag)
{
    _tag = tag;
}

QList<QSharedPointer<T>> DBHelper::load(const QString &path) {
    _path = path;
    QFile f(_path);
    f.open(QFile::ReadOnly);
    QTextStream in(&f);
    _doc.setContent(in.readAll());
    f.close();

    QList<QSharedPointer<T>> result;
    foreach(QDomElement &el, _doc.elementsByTagName(_tag)) {
        QSharedPointer<T> k(new T(el));
        result.append(k);
    }
    return result;
}

void DBHelper::save() {
    QString tmp_path = _path + ".tmp";
    QString bkp_path = Util::getNewBackupPath(_path);

    QFile f(tmp_path);
    f.open(QFile::WriteOnly);
    QTextStream out( &file );
    _doc.save(out, 4);
    f.close();

    // backup the original
    QFile::rename(_path, bkp_path);
    QFile::rename(tmp_path, _path);
}

QList<QSharedPointer<T>> DBHelper::restoreFromBackup() {
    QString bkp_path = Util::getLastBackupPath(_path);

    // replace with backup
    QFile::remove(_path);
    QFile::rename(bkp_path, _path);

    return load(_path);
}
