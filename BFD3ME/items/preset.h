#ifndef PRESET_H
#define PRESET_H

#include <QString>
#include <QDomElement>

#include "item.h"

/*
 * Again, boring...
 */

class Preset : public Item
{
private:
    QString _name;
    QString _libname;
    QString _libcode;
public:
    Preset(QDomElement &node);
    void setName(const QString &val);
    QString getName() const;
    void setLibname(const QString &val);
    QString getLibname() const;
    void setLibcode(const QString &val);
    QString getLibcode() const;
    QString getFilteredString() const;
    void save(QDomElement &node) const;
};

#endif // PRESET_H
