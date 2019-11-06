#include "ordenarintervencion.h"
#include "ui_ordenarintervencion.h"

OrdenarIntervencion::OrdenarIntervencion(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OrdenarIntervencion)
{
    ui->setupUi(this);
}

OrdenarIntervencion::~OrdenarIntervencion()
{
    delete ui;
}
