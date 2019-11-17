#include "editarusuario.h"
#include "ui_editarusuario.h"

EditarUsuario::EditarUsuario(QString matricula, QString typeU,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditarUsuario)
{
    ui->setupUi(this);
    ui->swEditar->setCurrentIndex(0);
    ui->btnSiguiente->hide();
    ui->btnGuardarCambios->hide();

    mat = matricula;
    tipoUsur = typeU;

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

    QString nombreUsr;
    QString appatUst;
    QString apmatUsr;
    QString email;
    QString telefono;
    QPixmap img;
    QString pregunta;
    QString respuesta;
    QString religion;
    QString sexo;

    QSqlQuery preguntas;
    preguntas.prepare("select * from pregunta");
    preguntas.exec();
    int i = 0;
    while(preguntas.next()){
        ui->cbPreguntas->insertItem(i,preguntas.value(1).toString());
        i++;
    }

    datosUsr = new QSqlQuery;

    if(tipoUsur == "doctor")
    {
        ui->btnSiguiente->show();
        qDebug() << "Buscamos doctor";
        datosUsr->prepare("SELECT nombre, appaterno, apmaterno, fechaN, edad, email, telefono, fotop, idpregunta, respuesta, religion, sexo FROM usuario WHERE matricula = "+mat+"");
        datosUsr->exec();
        datosUsr->next();
        nombreUsr = datosUsr->value(0).toString();
        appatUst = datosUsr->value(1).toString();
        apmatUsr = datosUsr->value(2).toString();
        email = datosUsr->value(5).toString();
        telefono = datosUsr->value(6).toString();
        img.loadFromData(datosUsr->value(7).toByteArray());
        pregunta = datosUsr->value(8).toString();
        respuesta = datosUsr->value(9).toString();
        religion = datosUsr->value(10).toString();
        sexo = datosUsr->value(11).toString();
        QSqlQuery doctorExtr;
        doctorExtr.prepare("SELECT especialidad.nombre as especialidad, horario, puesto.nombre as puesto, cedulapro, universidad, idconsultorio, estado FROM doctor INNER JOIN especialidad ON especialidad.idEsp = doctor.idEspecialidad INNER JOIN puesto ON puesto.idpuesto = doctor.idPuesto WHERE doctor.idUser = '"+mat+"'");
        doctorExtr.exec();
        doctorExtr.next();
    }
    else if (tipoUsur == "staff") {
        ui->btnSiguiente->show();
        datosUsr->prepare("SELECT nombre, appaterno, apmaterno, fechaN, edad, email, telefono, fotop, idpregunta, respuesta, religion, sexo FROM usuario WHERE matricula = '"+mat+"'");
        datosUsr->exec();
        datosUsr->next();
        qDebug() << "staff";
        nombreUsr = datosUsr->value(0).toString();
        appatUst = datosUsr->value(1).toString();
        apmatUsr = datosUsr->value(2).toString();
        email = datosUsr->value(5).toString();
        telefono = datosUsr->value(6).toString();
        img.loadFromData(datosUsr->value(7).toByteArray());
        pregunta = datosUsr->value(8).toString();
        respuesta = datosUsr->value(9).toString();
        religion = datosUsr->value(10).toString();
        sexo = datosUsr->value(11).toString();
        QSqlQuery staffExtr;
        staffExtr.prepare("SELECT puesto.nombre, areah.nombreArea, turno FROM staff INNER JOIN puesto on puesto.idpuesto = staff.idpuesto INNER JOIN areah on areah.idarea = staff.idArea WHERE idUser = '"+mat+"'");
        staffExtr.exec();
        staffExtr.next();
    }
    else if (tipoUsur == "paciente") {
        datosUsr->prepare("SELECT usuario.nombre, usuario.appaterno, usuario.apmaterno, usuario.fechaN, usuario.edad, usuario.email, usuario.telefono, usuario.fotop, usuario.idpregunta, usuario.respuesta, usuario.religion, usuario.sexo FROM paciente INNER JOIN usuario on paciente.idUser = usuario.matricula WHERE paciente.idUser = '"+mat+"'");
        datosUsr->exec();
        datosUsr->next();
        qDebug() << "paciente";
        nombreUsr = datosUsr->value(0).toString();
        appatUst = datosUsr->value(1).toString();
        apmatUsr = datosUsr->value(2).toString();
        email = datosUsr->value(5).toString();
        telefono = datosUsr->value(6).toString();
        img.loadFromData(datosUsr->value(7).toByteArray());
        pregunta = datosUsr->value(8).toString();
        respuesta = datosUsr->value(9).toString();
        religion = datosUsr->value(10).toString();
        sexo = datosUsr->value(11).toString();
        ui->btnGuardarCambios->show();
    }
    ui->lblFotoPerfil->setPixmap(img);
    ui->lnNombre->setText(nombreUsr);
    ui->lnPaterno->setText(appatUst);
    ui->lnMaterno->setText(apmatUsr);
    ui->lnCorreo->setText(email);
    ui->lnTel->setText(telefono);
    ui->cbPreguntas->setCurrentIndex(pregunta.toInt()-1);
    ui->lnRespuesta->setText(respuesta);
    ui->lnReligion->setText(religion);
    ui->lnSexo->setText(sexo);
}

EditarUsuario::~EditarUsuario()
{
    delete ui;
}

void EditarUsuario::on_btnCancelar_clicked()
{
    this->close();
}
