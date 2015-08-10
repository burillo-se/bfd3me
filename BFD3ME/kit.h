#ifndef KIT_H
#define KIT_H

#include <QString>
#include <QDomNode>

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
