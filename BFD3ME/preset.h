#ifndef PRESET_H
#define PRESET_H

#include <QString>
#include <QDomElement>

#include "util.h"

/*
 * Again, boring...
 */

class Preset
{
private:
    QString _name;
    QString _libname;
    QString _libcode;
    QDomElement _node;
    Util::FilterType _ftype;
public:
    Preset(QDomElement &node);
    void setName(const QString &val);
    QString getName() const;
    void setLibname(const QString &val);
    QString getLibname() const;
    void setLibcode(const QString &val);
    QString getLibcode() const;
    void setFilteredString(const Util::FilterType t);
    QString getFilteredString() const;
};

#endif // PRESET_H
