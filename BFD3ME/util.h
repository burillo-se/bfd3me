#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QDomElement>
#include <QObject>

/*
 * Utility class with common functions used by the program.
 */
#define KIT_TAG "BFD2KitInfo"
#define KITPIECE_TAG "BFD2KitPieceInfo"
#define PRESET_TAG "BFD2ProgramInfo"
#define KIT_FILTER "*.bfd3kit"
#define KITPIECE_FILTER "*Info.xml"
#define PRESET_FILTER "*.bfd3"
#define KIT_DB_FILENAME "Kit.database"
#define KITPIECE_DB_FILENAME "Kitpiece.database"
#define PRESET_DB_FILENAME "Programs.database"

class Util
{
public:
    static void setNodeAttr(QDomElement &node,
                            const QString &attr_name,
                            const QString &attr_val);
    static QString getNewBackupPath(QString &path);
    static QString getLastBackupPath(QString &path);
    static bool matchesFilter(const QString &val,
                              const QString &filter);
    enum Mode {
        Files,
        Database
    };
    enum Type {
        Kitpiece,
        Kit,
        Preset
    };
    enum FilterType {
        No_Filter,
        Name,
        Libname,
        Libcode,
        Class,
        Subclass,
        Manufacturer,
        Model,
        Date,
        Dimensions,
        Beater
    };
};

/*
 * Q_OBJECT macro needed for signals/slots support doesn't support templates,
 * so we create a dummy object with a signal, and then inherit our templates from it.
 */
class HelperBase : public QObject
{
    Q_OBJECT
protected:
    int _progressDone;
    int _progressTodo;
public:
    HelperBase(QObject *parent = 0) : QObject(parent) {}
signals:
    void progressChanged(QString,int,int);
    void finished();
    void error(QString,QString);
};

#endif // UTIL_H
