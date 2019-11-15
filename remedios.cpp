#include "remedios.h"
#include "ui_remedios.h"

remedios::remedios(QString tit ,QString ingr ,QString proced ,QByteArray fot ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::remedios)
{
    ui->setupUi(this);
    titulo=tit;
    ingredientes=ingr;
    procedimiento=proced;
    foto=fot;


}

remedios::~remedios()
{
    delete ui;
}

void remedios::llenarInformacion()
{
    ui->titulo_remedio->setText(titulo);
    ui->ingredientes_remedio->setPlainText(ingredientes);
    ui->procedimiento_remedio->setPlainText(procedimiento);
    ui->foto_remedio->setPixmap(foto);



}
