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

#ifndef BOILERPLATE_H
#define BOILERPLATE_H

/*
 * a header with boilerplate code macros.
 * yes, i'm a lazy person...
 */

#define CONCAT(a, b) a ## b

#define ITEM_PARAM(class_name, fn_name, param) \
void class_name::CONCAT(set, fn_name)(const QString &val) {\
    param = val; \
    if(!_saveToElement) \
        return; \
    Util::setNodeAttr(_node, CONCAT(_attr, param), param); \
} \
\
QString class_name::CONCAT(get, fn_name)() const {\
    return param;\
}

#define SAVE_PARAM(param) \
    Util::setNodeAttr(node, CONCAT(_attr, param), param)

#endif // BOILERPLATE_H

