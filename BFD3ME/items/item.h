#ifndef ITEM_H
#define ITEM_H

#include <QDomElement>

#include "utils/util.h"

/*
 * This is another contraption that i had to come with to support
 * two different methods of saving stuff.
 *
 * Basically, when a user sets a variable, two things may happen:
 * - if the "saveToElement" is "true", changes will be passed along to the
 *   QDomElement responsible for the element
 * - if it is "false", then user will have to call "save" to actually save
 *   the changes to XML.
 *
 * Being the contraption it is, all of this is up to subclasses to implement.
 */

class Item
{
protected:
    QDomElement _node;
    Util::FilterType _ftype;
    bool _saveToElement;
public:
    Item(QDomElement &node);
    void setFilteredString(const Util::FilterType t);
    void setSaveToElement(bool val);
    virtual QString getFilteredString() const = 0;
    virtual void save(QDomElement &node) const = 0;
};

#endif // ITEM_H
