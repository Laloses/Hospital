#include "administrarservicios.h"
#include "ui_administrarservicios.h"

AdministrarServicios::AdministrarServicios(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdministrarServicios)
{
    ui->setupUi(this);
}

AdministrarServicios::~AdministrarServicios()
{
    delete ui;
}
