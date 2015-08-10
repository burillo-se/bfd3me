#include "preset.h"
#include "util.h"

static const QString _name_attr = "info_name";
static const QString _libcode_attr = "info_library";
static const QString _libname_attr = "info_librarylong";


Preset::Preset(QDomElement &node)
{
    _node = node;
    _name = node.attribute(_name_attr);
    _libname = node.attribute(_libname_attr);
    _libcode = node.attribute(_libcode_attr);
}

void Preset::setName(const QString &val) {
    _name = val;
    Util::setNodeAttr(_node, _name_attr, _name);
}

QString Preset::getName() const {
    return _name;
}

void Preset::setLibname(const QString &val) {
    _libname = val;
    Util::setNodeAttr(_node, _libname_attr, _libname);
}

QString Preset::getLibname() const {
    return _libname;
}

void Preset::setLibcode(const QString &val) {
    _libcode = val;
    Util::setNodeAttr(_node, _libcode_attr, _libcode);
}

QString Preset::getLibcode() const {
    return _libcode;
}
