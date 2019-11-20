#include "infohospital.h"
#include "ui_infohospital.h"

InfoHospital::~InfoHospital()
{
    delete ui;
}

InfoHospital::InfoHospital(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoHospital)
{
    ui->setupUi(this);
}

void InfoHospital::on_pb_acerca_clicked()
{
    ui->tituloSec->setText("¡Bienvenido a LoboHospital!");
    ui->sw_secciones->setCurrentIndex(0);

    this->repaint();
}

void InfoHospital::on_pb_MyV_clicked()
{
    ui->tituloSec->setText("Lobo-Vision y Lobo-Mision");
    ui->sw_secciones->setCurrentIndex(1);

    this->repaint();
}

void InfoHospital::on_pb_valores_clicked()
{
    ui->tituloSec->setText("Lobo-Valores");
    ui->sw_secciones->setCurrentIndex(2);

    this->repaint();
}

void InfoHospital::on_pb_calidad_clicked()
{
    ui->tituloSec->setText("Lobo-políticas de calidad");
    ui->sw_secciones->setCurrentIndex(3);

    this->repaint();
}
