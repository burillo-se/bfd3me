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
    static QString getNewBackupPath(const QString &path);
    static QString getLastBackupPath(const QString &path);
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

#endif // UTIL_H
