#ifndef KIT_H
#define KIT_H

#include <QString>
#include <QDomElement>

/*
 * Class representing a Kit. Boring...
 */

class Kit
{
private:
    QString _name;
    QString _libname;
    QString _libcode;
    QDomElement _node;
public:
    Kit(QDomElement &node);
    void setName(const QString &val);
    QString getName() const;
    void setLibname(const QString &val);
    QString getLibname() const;
    void setLibcode(const QString &val);
    QString getLibcode() const;
};

#endif // KIT_H
