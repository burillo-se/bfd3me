#include "kit.h"
#include "util.h"

#include "boilerplate.h"

/*
 * More boilerplate code...
 */

static const QString _attr_name = "info_name";
static const QString _attr_libcode = "info_library";
static const QString _attr_libname = "info_librarylong";

Kit::Kit(QDomElement &node)
{
    _node = node;
    _name = node.attribute(_attr_name);
    _libname = node.attribute(_attr_libname);
    _libcode = node.attribute(_attr_libcode);
}

ITEM_PARAM(Kit, Name, _name)
ITEM_PARAM(Kit, Libname, _libname)
ITEM_PARAM(Kit, Libcode, _libcode)

QString Kit::getFilteredString() const {
    switch (_ftype) {
    case Util::Name:
        return _name;
    case Util::Libcode:
        return _libcode;
    case Util::Libname:
        return _libname;
    }
    // can't happen
    return QString();
}

void Kit::setFilteredString(const Util::FilterType t) {
    _ftype = t;
}
