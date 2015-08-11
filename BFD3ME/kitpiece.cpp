#include "kitpiece.h"
#include "util.h"

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

static QDomElement getClassNode(QDomElement &node) {
    QDomNodeList list = node.elementsByTagName("VString");
    for (int i = 0; i < list.count(); i++) {
        QDomNode class_node = list[i];
        if (class_node.attribute("name") == _class_attr) {
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

void Kitpiece::setName(const QString &val) {
    _name = val;
    Util::setNodeAttr(_node, _name_attr, _name);
}

QString Kitpiece::getName() const {
    return _name;
}

void Kitpiece::setLibname(const QString &val) {
    _libname = val;
    Util::setNodeAttr(_node, _libname_attr, _libname);
}

QString Kitpiece::getLibname() const {
    return _libname;
}

void Kitpiece::setLibcode(const QString &val) {
    _libcode = val;
    Util::setNodeAttr(_node, _libcode_attr, _libcode);
}

QString Kitpiece::getLibcode() const {
    return _libcode;
}

void Kitpiece::setManufacturer(const QString &val) {
    _manufacturer = val;
    Util::setNodeAttr(_node, _manufacturer_attr, _manufacturer);
}

QString Kitpiece::getManufacturer() const {
    return _manufacturer;
}

void Kitpiece::setModel(const QString &val) {
    _model = val;
    Util::setNodeAttr(_node, _model_attr, _model);
}

QString Kitpiece::getModel() const {
    return _model;
}

void Kitpiece::setDate(const QString &val) {
    _date = val;
    Util::setNodeAttr(_node, _date_attr, _date);
}

QString Kitpiece::getDate() const {
    return _date;
}

void Kitpiece::setDimensions(const QString &val) {
    _dimensions = val;
    Util::setNodeAttr(_node, _dimensions_attr, _dimensions);
}

QString Kitpiece::getDimensions() const {
    return _dimensions;
}

void Kitpiece::setBeater(const QString &val) {
    _beater = val;
    Util::setNodeAttr(_node, _beater_attr, _beater);
}

QString Kitpiece::getBeater() const {
    return _beater;
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

void Kitpiece::setSubclass(const QString &val) {
    _subclass = val;
    Util::setNodeAttr(_node, _subclass_attr, _subclass);
}

QString Kitpiece::getSubclass() const {
    return _subclass;
}
