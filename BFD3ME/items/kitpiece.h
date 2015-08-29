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

#ifndef KITPIECE_H
#define KITPIECE_H

#include <QString>
#include <QDomElement>

#include "item.h"

/*
 * Kitpiece got a few more parameters, but overall, same as others.
 */

class Kitpiece : public Item
{
private:
    QString _name;
    QString _libname;
    QString _libcode;
    QString _manufacturer;
    QString _model;
    QString _date;
    QString _dimensions;
    QString _beater;
    QString _class;
    QString _subclass;
    QString _path;
public:
    Kitpiece(QDomElement &node);
    void setName(const QString &val);
    QString getName() const;
    void setLibname(const QString &val);
    QString getLibname() const;
    void setLibcode(const QString &val);
    QString getLibcode() const;
    void setManufacturer(const QString &val);
    QString getManufacturer() const;
    void setModel(const QString &val);
    QString getModel() const;
    void setDate(const QString &val);
    QString getDate() const;
    void setDimensions(const QString &val);
    QString getDimensions() const;
    void setBeater(const QString &val);
    QString getBeater() const;
    void setClass(const QString &val);
    QString getClass() const;
    void setSubclass(const QString &val);
    QString getPath() const;
    void setPath(const QString &val);
    QString getSubclass() const;
    QString getFilteredString() const;
    void save(QDomElement &node) const;
};

#endif // KITPIECE_H
