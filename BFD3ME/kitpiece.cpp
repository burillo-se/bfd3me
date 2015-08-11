#include "kitpiece.h"
#include "util.h"
#include "boilerplate.h"

/*
 * Mostly boring boilerplate code, but class is a special case.
 */

static const QString _name_attr = "kpi_name";
static const QString _libcode_attr = "info_library";
static const QString _libname_attr = "info_librarylong";
static const QString _manufacturer_attr = "kpi_manufacturer";
static const QString _model_attr = "kpi_model";
static const QString _date_attr = "kpi_date";
static const QString _dimensions_attr = "kpi_dimensions";
static const QString _beater_attr = "kpi_beater";
static const QString _class_attr = "kpi_class";
static const QString _subclass_attr = "kpi_subclass";

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
        if (!class_node.isNull() && class_node.attribute("name") == _class_attr) {
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
Kitpiece::Kitpiece(QDomElement &node)
{
    _node = node;
    _name = node.attribute(_name_attr);
    _libname = node.attribute(_libname_attr);
    _libcode = node.attribute(_libcode_attr);
    _subclass = node.attribute(_subclass_attr);
    _manufacturer = node.attribute(_manufacturer_attr);
    _model = node.attribute(_model_attr);
    _date = node.attribute(_date_attr);
    _dimensions = node.attribute(_dimensions_attr);
    _beater = node.attribute(_beater_attr);

    /* class is a special case */
    QDomElement class_node = getClassNode(_node);
    _class = class_node.attribute("string");
}

void Kitpiece::setClass(const QString &val) {
    _class = val;
    /* class is a special case */
    QDomElement class_node = getClassNode(_node);
    Util::setNodeAttr(class_node, "string", _class);
}

QString Kitpiece::getClass() const {
    return _class;
}

ITEM_PARAM(Kitpiece, setName, getName, _name, _name_attr)
ITEM_PARAM(Kitpiece, setLibname, getLibname, _libname, _libname_attr)
ITEM_PARAM(Kitpiece, setLibcode, getLibcode, _libcode, _libcode_attr)
ITEM_PARAM(Kitpiece, setSubclass, getSubclass, _subclass, _subclass_attr)
ITEM_PARAM(Kitpiece, setManufacturer, getManufacturer, _manufacturer, _manufacturer_attr)
ITEM_PARAM(Kitpiece, setModel, getModel, _model, _model_attr)
ITEM_PARAM(Kitpiece, setDate, getDate, _date, _date_attr)
ITEM_PARAM(Kitpiece, setDimensions, getDimensions, _dimensions, _dimensions_attr)
ITEM_PARAM(Kitpiece, setBeater, getBeater, _beater, _beater_attr)
