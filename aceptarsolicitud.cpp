#include "aceptarsolicitud.h"
#include "ui_aceptarsolicitud.h"

aceptarSolicitud::aceptarSolicitud(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aceptarSolicitud)
{
    ui->setupUi(this);
}

aceptarSolicitud::~aceptarSolicitud()
{
    delete ui;
}
