#include "eliminarusuarios.h"
#include "ui_eliminarusuarios.h"

eliminarUsuarios::eliminarUsuarios(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::eliminarUsuarios)
{
    ui->setupUi(this);
}

eliminarUsuarios::~eliminarUsuarios()
{
    delete ui;
}
