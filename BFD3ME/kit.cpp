#include "kit.h"
#include "util.h"

#include "boilerplate.h"

/*
 * More boilerplate code...
 */

static const QString _name_attr = "info_name";
static const QString _libcode_attr = "info_library";
static const QString _libname_attr = "info_librarylong";

Kit::Kit(QDomElement &node)
{
    _node = node;
    _name = node.attribute(_name_attr);
    _libname = node.attribute(_libname_attr);
    _libcode = node.attribute(_libcode_attr);
}

ITEM_PARAM(Kit, setName, getName, _name, _name_attr)
ITEM_PARAM(Kit, setLibname, getLibname, _libname, _libname_attr)
ITEM_PARAM(Kit, setLibcode, getLibcode, _libcode, _libcode_attr)
