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
    calidad="Lobo-políticas de calidad";
    ui->tituloSec->setText("¡Bienvenido a LoboHospital!");
    ui->tituloSec->setText("Lobo.Vision y Lobo-Mision");
    ui->tituloSec->setText("Lobo-Valores");
}

void InfoHospital::on_pb_acerca_clicked()
{
    ui->sw_secciones->setCurrentIndex(0);
}

void InfoHospital::on_pb_MyV_clicked()
{
    ui->sw_secciones->setCurrentIndex(1);
}

void InfoHospital::on_pb_valores_clicked()
{
    ui->sw_secciones->setCurrentIndex(1);
}
