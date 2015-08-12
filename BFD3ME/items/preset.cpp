#include "items/preset.h"
#include "utils/util.h"
#include "utils/boilerplate.h"

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
    }
    // can't happen
    return QString();
}

void Preset::save(QDomElement &node) const {
    SAVE_PARAM(_name);
    SAVE_PARAM(_libname);
    SAVE_PARAM(_libcode);
}
