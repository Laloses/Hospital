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
    QSqlQuery* datos = new QSqlQuery(QSqlDatabase::database());
    if( !datos->exec("SELECT * FROM infoHospital WHERE 1") )
        qDebug()<<datos->lastError().text();
    if( !datos->next() )
        qDebug()<<datos->lastError().text();
    ui->acercaDe->setPlainText(datos->value("acercaDe").toString());
    ui->mision->setPlainText(datos->value("mision").toString());
    ui->vision->setPlainText(datos->value("vision").toString());
    ui->valores->setPlainText(datos->value("valores").toString());
    ui->calidad->setPlainText(datos->value("calidad").toString());
    on_pb_acerca_clicked();
}

void InfoHospital::on_pb_acerca_clicked()
{
    ui->tituloSec->setText("¡Bienvenido a LoboHospital!");
    ui->sw_secciones->setCurrentIndex(0);

    //this->setFixedSize(this->width(),480);
}

void InfoHospital::on_pb_MyV_clicked()
{
    ui->tituloSec->setText("Lobo-Vision y Lobo-Mision");
    ui->sw_secciones->setCurrentIndex(1);

    //this->setFixedSize(this->width(),500);
}

void InfoHospital::on_pb_valores_clicked()
{
    ui->tituloSec->setText("Lobo-Valores");
    ui->sw_secciones->setCurrentIndex(2);

    //this->setFixedSize(this->width(),480);
}

void InfoHospital::on_pb_calidad_clicked()
{
    ui->tituloSec->setText("Lobo-políticas de calidad");
    ui->sw_secciones->setCurrentIndex(3);

    //this->setFixedSize(this->width(),480);
}
