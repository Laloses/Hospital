#include "tipdeldia.h"
#include "ui_tipdeldia.h"

tipdeldia::tipdeldia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tipdeldia)
{
    ui->setupUi(this);
}

tipdeldia::~tipdeldia()
{
    delete ui;
}
