#ifndef KIT_H
#define KIT_H

#include <QString>
#include <QDomElement>

#include "util.h"
#include "item.h"

/*
 * Class representing a Kit. Boring...
 */

class Kit : public Item
{
private:
    QString _name;
    QString _libname;
    QString _libcode;
public:
    Kit(QDomElement &node);
    void setName(const QString &val);
    QString getName() const;
    void setLibname(const QString &val);
    QString getLibname() const;
    void setLibcode(const QString &val);
    QString getLibcode() const;
    QString getFilteredString() const;
    void save(QDomElement &node) const;
};

#endif // KIT_H
