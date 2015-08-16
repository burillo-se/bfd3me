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

#ifndef ITEM_H
#define ITEM_H

#include <QDomElement>

#include "utils/util.h"

/*
 * This is another contraption that i had to come with to support
 * two different methods of saving stuff.
 *
 * Basically, when a user sets a variable, two things may happen:
 * - if the "saveToElement" is "true", changes will be passed along to the
 *   QDomElement responsible for the element
 * - if it is "false", then user will have to call "save" to actually save
 *   the changes to XML.
 *
 * Being the contraption it is, all of this is up to subclasses to implement.
 */

class Item
{
protected:
    QDomElement _node;
    Util::FilterType _ftype;
    bool _saveToElement;
public:
    Item(QDomElement &node);
    void setFilteredString(const Util::FilterType t);
    void setSaveToElement(bool val);
    virtual QString getFilteredString() const = 0;
    virtual void save(QDomElement &node) const = 0;
    virtual ~Item() = 0;
};

#endif // ITEM_H
