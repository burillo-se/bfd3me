#ifndef KITPIECE_H
#define KITPIECE_H

#include <QString>
#include <QDomElement>

#include "util.h"

/*
 * Kitpiece got a few more parameters, but overall, same as others.
 */

class Kitpiece
{
private:
    QString _name;
    QString _libname;
    QString _libcode;
    QString _manufacturer;
    QString _model;
    QString _date;
    QString _dimensions;
    QString _beater;
    QString _class;
    QString _subclass;
    QDomElement _node;
    Util::FilterType _ftype;
public:
    Kitpiece(QDomElement &node);
    void setName(const QString &val);
    QString getName() const;
    void setLibname(const QString &val);
    QString getLibname() const;
    void setLibcode(const QString &val);
    QString getLibcode() const;
    void setManufacturer(const QString &val);
    QString getManufacturer() const;
    void setModel(const QString &val);
    QString getModel() const;
    void setDate(const QString &val);
    QString getDate() const;
    void setDimensions(const QString &val);
    QString getDimensions() const;
    void setBeater(const QString &val);
    QString getBeater() const;
    void setClass(const QString &val);
    QString getClass() const;
    void setSubclass(const QString &val);
    QString getSubclass() const;
    void setFilteredString(const Util::FilterType t);
    QString getFilteredString() const;
};

#endif // KITPIECE_H
