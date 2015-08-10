#include "bfd3me.h"
#include "ui_bfd3me.h"

BFD3ME::BFD3ME(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BFD3ME)
{
    ui->setupUi(this);
}

BFD3ME::~BFD3ME()
{
    delete ui;
}
