#include "cambiarcontrasenia.h"
#include "ui_cambiarcontrasenia.h"

CambiarContrasenia::CambiarContrasenia(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CambiarContrasenia)
{
    ui->setupUi(this);
}

CambiarContrasenia::~CambiarContrasenia()
{
    delete ui;
}
