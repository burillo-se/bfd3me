#include "item.h"

#include "util.h"

Item::Item(QDomElement &node)
{
    _node = node;
    _saveToElement = true;
}

void Item::setFilteredString(const Util::FilterType t) {
    _ftype = t;
}

void Item::setSaveToElement(bool val) {
    _saveToElement = val;

    // if we don't want direct saving, clear the memory
    if (!_saveToElement)
        _node = QDomElement();
}
