#include "bfd3me.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BFD3ME w;
    w.show();

    return a.exec();
}
