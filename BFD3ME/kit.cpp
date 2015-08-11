#include "kit.h"
#include "util.h"

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

void Kit::setName(const QString &val) {
    _name = val;
    Util::setNodeAttr(_node, _name_attr, _name);
}

QString Kit::getName() const {
    return _name;
}

void Kit::setLibname(const QString &val) {
    _libname = val;
    Util::setNodeAttr(_node, _libname_attr, _libname);
}

QString Kit::getLibname() const {
    return _libname;
}

void Kit::setLibcode(const QString &val) {
    _libcode = val;
    Util::setNodeAttr(_node, _libcode_attr, _libcode);
}

QString Kit::getLibcode() const {
    return _libcode;
}
