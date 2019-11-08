#include "agregarUrgencia.h"
#include "ui_agregarUrgencia.h"

agregarUrgencia::agregarUrgencia(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::agregarUrgencia)
{
    ui->setupUi(this);
}

agregarUrgencia::~agregarUrgencia()
{
    delete ui;
}
