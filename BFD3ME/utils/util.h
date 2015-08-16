/***
* BFD3 Metadata Editor
* Copyright (C) 2015 Anatoly Burakov
* 
* Contact information: burakov.anatoly@gmail.com
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

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
#define KITPIECE_DB_FILENAME "KitPiece.database"
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
