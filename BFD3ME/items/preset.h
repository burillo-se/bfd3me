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

#ifndef PRESET_H
#define PRESET_H

#include <QString>
#include <QDomElement>

#include "item.h"

/*
 * Again, boring...
 */

class Preset : public Item
{
private:
    QString _name;
    QString _libname;
    QString _libcode;
    QString _path;
public:
    Preset(QDomElement &node);
    void setName(const QString &val);
    QString getName() const;
    void setLibname(const QString &val);
    QString getLibname() const;
    void setLibcode(const QString &val);
    QString getLibcode() const;
    void setPath(const QString &val);
    QString getPath() const;
    QString getFilteredString() const;
    void save(QDomElement &node) const;
};

#endif // PRESET_H
