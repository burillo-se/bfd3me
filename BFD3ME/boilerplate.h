#ifndef BOILERPLATE_H
#define BOILERPLATE_H

/*
 * a header with boilerplate code macros.
 * yes, i'm a lazy person...
 */

#define ITEM_PARAM(class_name, fn_name, param) \
void class_name::set##fn_name(const QString &val) {\
    param = val; \
    Util::setNodeAttr(_node, _attr##param, param); \
} \
\
QString class_name::get##fn_name() const {\
    return param;\
}

#endif // BOILERPLATE_H

