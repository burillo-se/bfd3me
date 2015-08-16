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

#include "items/preset.h"
#include "utils/util.h"
#include "utils/boilerplate.h"

/*
 * Boring, nothing to see here.
 */

#include <QtDebug>

static const QString _attr_name = "info_name";
static const QString _attr_libcode = "info_library";
static const QString _attr_libname = "info_librarylong";

/*
 * XML parsing is in the constructor. What happens if we
 * can't parse? I dunno... Maybe later i'll add exceptions
 */
Preset::Preset(QDomElement &node) : Item(node)
{
    _name = node.attribute(_attr_name);
    _libname = node.attribute(_attr_libname);
    _libcode = node.attribute(_attr_libcode);
}

ITEM_PARAM(Preset, Name, _name)
ITEM_PARAM(Preset, Libname, _libname)
ITEM_PARAM(Preset, Libcode, _libcode)

QString Preset::getFilteredString() const {
    switch (_ftype) {
    case Util::Name:
        return _name;
    case Util::Libcode:
        return _libcode;
    case Util::Libname:
        return _libname;
    default:
        return QString();
    }
    // can't happen
    return QString();
}

void Preset::save(QDomElement &node) const {
    SAVE_PARAM(_name);
    SAVE_PARAM(_libname);
    SAVE_PARAM(_libcode);
}
