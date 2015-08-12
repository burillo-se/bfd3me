#ifndef HELPERBASE_H
#define HELPERBASE_H

#include <QObject>
#include <QDomDocument>
#include <QString>

/*
 * Q_OBJECT macro needed for signals/slots support doesn't support templates,
 * so we create a dummy object with a signal, and then inherit our templates from it.
 */
class HelperBase : public QObject
{
    Q_OBJECT
protected:
    QDomDocument loadDoc(const QString &path);
    void saveDoc(const QDomDocument &doc, const QString &path);
    void restoreFile(const QString &path);
    int _progressDone;
    int _progressTodo;
public:
    HelperBase(QObject *parent = 0) : QObject(parent) {}
signals:
    void progressChanged(QString,int,int);
    void finished();
    void error(QString,QString);
};

#endif // HELPERBASE_H
