#include "preset.h"
#include "util.h"
#include "boilerplate.h"

/*
 * Boring, nothing to see here.
 */

static const QString _attr_name = "info_name";
static const QString _attr_libcode = "info_library";
static const QString _attr_libname = "info_librarylong";

/*
 * XML parsing is in the constructor. What happens if we
 * can't parse? I dunno... Maybe later i'll add exceptions
 */
Preset::Preset(QDomElement &node)
{
    _node = node;
    _name = node.attribute(_attr_name);
    _libname = node.attribute(_attr_libname);
    _libcode = node.attribute(_attr_libcode);
}

ITEM_PARAM(Preset, Name, _name)
ITEM_PARAM(Preset, Libname, _libname)
ITEM_PARAM(Preset, Libcode, _libcode)
