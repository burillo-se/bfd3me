#include "kitpiece.h"
#include "util.h"
#include "boilerplate.h"

/*
 * Mostly boring boilerplate code, but class is a special case.
 */

static const QString _attr_name = "kpi_name";
static const QString _attr_libcode = "info_library";
static const QString _attr_libname = "info_librarylong";
static const QString _attr_manufacturer = "kpi_manufacturer";
static const QString _attr_model = "kpi_model";
static const QString _attr_date = "kpi_date";
static const QString _attr_dimensions = "kpi_dimensions";
static const QString _attr_beater = "kpi_beater";
static const QString _attr_class = "kpi_class";
static const QString _attr_subclass = "kpi_subclass";

/*
 * In a kitpiece, class node is a special case.
 *
 * Long story short, we need to find a VString node with "name" attribute
 * set to "kpi_class". Because this is how item class looks like. Not sure
 * why class wasn't made an attribute like the rest of the stuff, you'd
 * have to ask FXpansion about that one...
 */
static QDomElement getClassNode(QDomElement &node) {
    QDomNodeList list = node.elementsByTagName("VString");
    for (int i = 0; i < list.count(); i++) {
        QDomElement class_node = list.at(i).toElement();
        if (!class_node.isNull() && class_node.attribute("name") == _attr_class) {
            return class_node.toElement();
        }
    }
    // if we're here, that means we didn't find a class node, so create it
    QDomElement el = node.ownerDocument().createElement("VString");
    el.setAttribute("name", "kpi_class");
    el.setAttribute("string", "");
    node.appendChild(el);
    return el;
}

/*
 * XML parsing is in the constructor. What happens if we
 * can't parse? I dunno... Maybe later i'll add exceptions
 */
Kitpiece::Kitpiece(QDomElement &node) : Item(node)
{
    _name = node.attribute(_attr_name);
    _libname = node.attribute(_attr_libname);
    _libcode = node.attribute(_attr_libcode);
    _subclass = node.attribute(_attr_subclass);
    _manufacturer = node.attribute(_attr_manufacturer);
    _model = node.attribute(_attr_model);
    _date = node.attribute(_attr_date);
    _dimensions = node.attribute(_attr_dimensions);
    _beater = node.attribute(_attr_beater);

    /* class is a special case */
    QDomElement class_node = getClassNode(_node);
    _class = class_node.attribute("string");
}

void Kitpiece::setClass(const QString &val) {
    _class = val;
    if(!_saveToElement)
        return;

    /* class is a special case */
    QDomElement class_node = getClassNode(_node);
    Util::setNodeAttr(class_node, "string", _class);
}

QString Kitpiece::getClass() const {
    return _class;
}

ITEM_PARAM(Kitpiece, Name, _name)
ITEM_PARAM(Kitpiece, Libname, _libname)
ITEM_PARAM(Kitpiece, Libcode, _libcode)
ITEM_PARAM(Kitpiece, Subclass, _subclass)
ITEM_PARAM(Kitpiece, Manufacturer, _manufacturer)
ITEM_PARAM(Kitpiece, Model, _model)
ITEM_PARAM(Kitpiece, Date, _date)
ITEM_PARAM(Kitpiece, Dimensions, _dimensions)
ITEM_PARAM(Kitpiece, Beater, _beater)

QString Kitpiece::getFilteredString() const {
    switch (_ftype) {
    case Util::Name:
        return _name;
    case Util::Libcode:
        return _libcode;
    case Util::Libname:
        return _libname;
    case Util::Manufacturer:
        return _manufacturer;
    case Util::Model:
        return _model;
    case Util::Date:
        return _date;
    case Util::Dimensions:
        return _dimensions;
    case Util::Beater:
        return _beater;
    case Util::Class:
        return _class;
    case Util::Subclass:
        return _subclass;
    }
    // can't happen
    return QString();
}

void Kitpiece::save(QDomElement &node) const {
    SAVE_PARAM(_name);
    SAVE_PARAM(_libname);
    SAVE_PARAM(_libcode);
    SAVE_PARAM(_manufacturer);
    SAVE_PARAM(_date);
    SAVE_PARAM(_model);
    SAVE_PARAM(_dimensions);
    SAVE_PARAM(_beater);
    SAVE_PARAM(_subclass);

    // class is a special case
    QDomElement class_node = getClassNode(node);
    Util::setNodeAttr(class_node, "string", _class);
}
