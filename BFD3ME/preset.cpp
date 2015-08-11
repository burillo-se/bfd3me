#include "preset.h"
#include "util.h"
#include "boilerplate.h"

/*
 * Boring, nothing to see here.
 */

static const QString _name_attr = "info_name";
static const QString _libcode_attr = "info_library";
static const QString _libname_attr = "info_librarylong";

/*
 * XML parsing is in the constructor. What happens if we
 * can't parse? I dunno... Maybe later i'll add exceptions
 */
Preset::Preset(QDomElement &node)
{
    _node = node;
    _name = node.attribute(_name_attr);
    _libname = node.attribute(_libname_attr);
    _libcode = node.attribute(_libcode_attr);
}

ITEM_PARAM(Preset, setName, getName, _name, _name_attr)
ITEM_PARAM(Preset, setLibname, getLibname, _libname, _libname_attr)
ITEM_PARAM(Preset, setLibcode, getLibcode, _libcode, _libcode_attr)
