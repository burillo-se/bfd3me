#ifndef BFD3ME_H
#define BFD3ME_H

#include <QMainWindow>

namespace Ui {
class BFD3ME;
}

class BFD3ME : public QMainWindow
{
    Q_OBJECT

public:
    explicit BFD3ME(QWidget *parent = 0);
    ~BFD3ME();

private:
    Ui::BFD3ME *ui;
};

#endif // BFD3ME_H
