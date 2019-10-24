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

aceptarSolicitud::aceptarSolicitud(QString hostName, int port, QString dbName, QString userName, QString pass){
    database=QSqlDatabase::database();
    database= QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName(hostName);
    database.setPort(port);
    database.setDatabaseName(dbName);
    database.setUserName(userName);
    database.setPassword(pass);
    database.open();

}

void aceptarSolicitud::mostrarZonas(){
    database.open();
    QSqlQueryModel *queryPuestos;
    queryPuestos= new QSqlQueryModel;
    queryPuestos->setQuery("SELECT nombreArea FROM areah");
    ui->comboBox_area->setModel(queryPuestos);


}

void aceptarSolicitud::on_pushButton_guardar_clicked()
{

    close();

}
