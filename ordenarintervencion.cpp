#include "ordenarintervencion.h"
#include "ui_ordenarintervencion.h"

#include <QString>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

OrdenarIntervencion::OrdenarIntervencion(QString idCita, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OrdenarIntervencion)
{
    ui->setupUi(this);
    idCita1 = idCita;

    //Conexion a la base de datos
    database= QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("localhost");
    database.setPort(3306);
    database.setDatabaseName("lobohospital");
    database.setUserName("root");
    database.setPassword("");
    if(!database.open()){
        qDebug()<<database.lastError().text();
    }
    else {
        qDebug()<<"Base de datos conectada";
    }

    datosPaciente = new QSqlQuery;
    datosPaciente->prepare("SELECT CONCAT_WS( ' ', usuario.nombre, usuario.appaterno, usuario.apmaterno ) AS Paciente FROM cita INNER JOIN paciente ON paciente.idpaciente = cita.doctor INNER JOIN usuario ON paciente.idUser = usuario.matricula WHERE cita.idCita ='"+idCita+"'");
    datosPaciente->exec();
    datosPaciente->next();
    QString paciente = datosPaciente->value(0).toString();
    datosDoctor = new QSqlQuery;
    datosDoctor->prepare("SELECT CONCAT_WS( ' ', usuario.nombre, usuario.appaterno, usuario.apmaterno ) AS Doctor FROM cita INNER JOIN doctor ON doctor.iddoctor = cita.doctor INNER JOIN usuario ON doctor.idUser = usuario.matricula WHERE cita.idCita ='"+idCita+"'");
    datosDoctor->exec();
    datosDoctor->next();
    QString doctor = datosDoctor->value(0).toString();

    ui->intervencionPaciente->setText(paciente);
    ui->intervencionNombreDoc->setText(doctor);

    //timer precios
    QStringList wordList;

        QString busqueda,valor;
        QSqlQuery busq;
        busqueda="select *from Producto";
        busq.exec(busqueda);

        while(busq.next())
        {
            valor=busq.value(1).toString();
            wordList << valor;
        }

        QCompleter *completer = new QCompleter(wordList, this);
            completer->setCaseSensitivity(Qt::CaseInsensitive);

            ui->lineServ->setCompleter(completer);

            cuenta=new QTimer(this);
               connect(cuenta, SIGNAL(timeout()), this, SLOT(precio()));
                cuenta->start(100);
    //---------------------------------------------------------
}

OrdenarIntervencion::~OrdenarIntervencion()
{
    delete ui;
}

void OrdenarIntervencion::on_btnContinuarInter_clicked()
{
    ui->tabWidget->setCurrentIndex(1);
}

void OrdenarIntervencion::on_btnBackInter_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}

void OrdenarIntervencion::on_btnCancelarInter_clicked()
{
    this->close();
}

void OrdenarIntervencion::precio()
{
    QString Busq,costo,productoo;
    productoo=ui->lineServ->text();
    QSqlQuery busca1;
    Busq="Select precio from Producto where nombre='"+productoo+"'";
    busca1.exec(Busq);
    busca1.next();
    costo=busca1.value(0).toString();
     ui->lineCosto->setText(costo);

}

void OrdenarIntervencion::on_lineServ_editingFinished()
{
    cuenta->stop();
}

void OrdenarIntervencion::on_lineServ_textChanged(const QString &arg1)
{
    cuenta->start(100);
}
