#ifndef DBHELPER_H
#define DBHELPER_H

#include <QString>
#include <QDomDocument>
#include <QList>
#include <QSharedPointer>

template <typename T>
class DBHelper
{
private:
    QString _tag;
    QString _path;
    QDomDocument _doc;
public:
    DBHelper(const QString &tag);
    QList<QSharedPointer<T>> load(const QString &path);
    void save();
    QList<QSharedPointer<T>> restoreFromBackup();
};

template <typename T>
DBHelper<T>::DBHelper(const QString &tag)
{
    _tag = tag;
}

template <typename T>
QList<QSharedPointer<T>> DBHelper<T>::load(const QString &path) {
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

template <typename T>
void DBHelper<T>::save() {
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

template <typename T>
QList<QSharedPointer<T>> DBHelper<T>::restoreFromBackup() {
    QString bkp_path = Util::getLastBackupPath(_path);

    // replace with backup
    QFile::remove(_path);
    QFile::rename(bkp_path, _path);

    return load(_path);
}


#endif // DBHELPER_H
