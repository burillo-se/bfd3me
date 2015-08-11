#ifndef BOILERPLATE_H
#define BOILERPLATE_H

/*
 * a header with boilerplate code macros.
 * yes, i'm a lazy person...
 */

#define ITEM_PARAM(class_name, mut_fn, acc_fn, param, param_str) \
void class_name::mut_fn(const QString &val) {\
    param = val; \
    Util::setNodeAttr(_node, param_str, param); \
} \
\
QString class_name::acc_fn() const {\
    return param;\
}

#endif // BOILERPLATE_H

