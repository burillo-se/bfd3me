#include "helperbase.h"

#include <QTextStream>
#include <QFile>

#include "util.h"

/*
 * Get a QDomDocument given a file path
 */
QDomDocument HelperBase::loadDoc(const QString &path) {
    QDomDocument doc;
    QFile f(path);
    f.open(QFile::ReadOnly);

    QString errorStr;
    int line, col;

    QTextStream in(&f);
    if (!doc.setContent(in.readAll(), false, &errorStr, &line, &col)) {
        emit error(path, QString("Line %0 column %1: %2").arg(line).arg(col).arg(errorStr));
    }
    f.close();

    return doc;
}

/*
 * Save a QDomDocument to a given path
 */
void HelperBase::saveDoc(const QDomDocument &doc, const QString &path) {
    QString tmp_path = path + ".tmp";
    QString bkp_path = Util::getNewBackupPath(path);

    QFile f(tmp_path);
    f.open(QFile::WriteOnly);
    QTextStream out( &f );
    doc.save(out, 4);
    f.close();

    // backup the original
    QFile::rename(path, bkp_path);
    QFile::rename(tmp_path, path);
}

void HelperBase::restoreFile(const QString &path) {
    QString bkp_path = Util::getLastBackupPath(path);

    // replace with backup
    if (bkp_path != path) {
        QFile::remove(path);
        QFile::rename(bkp_path, path);
    }
}
