#include "editarusuario.h"
#include "ui_editarusuario.h"

EditarUsuario::EditarUsuario(QString matricula, QString typeU,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditarUsuario)
{
    ui->setupUi(this);

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

    datosUsr = new QSqlQuery;
    if(tipoUsur == "doctor")
    {
        datosUsr->prepare("SELECT `matricula`, concat_ws(' ', `nombre`, `appaterno`, `apmaterno`) as Nombre, `fechaN`, `edad`, `email`, `telefono`, `fotop`, `idpregunta`, `respuesta`, `religion`, `sexo` FROM `usuario` WHERE matricula = '"+mat+"'");
        datosUsr->exec();
        QSqlQuery doctorExtr;
        doctorExtr.prepare("SELECT especialidad.nombre as especialidad, `horario`, puesto.nombre as puesto, `cedulapro`, `universidad`, `idconsultorio`, `estado` FROM `doctor` INNER JOIN especialidad ON especialidad.idEsp = doctor.idEspecialidad INNER JOIN puesto ON puesto.idpuesto = doctor.idPuesto WHERE doctor.idUser = '"+mat+"'");
        doctorExtr.exec();
    }
    else if (tipoUsur == "staff") {
        datosUsr->prepare("SELECT `matricula`, concat_ws(' ', `nombre`, `appaterno`, `apmaterno`) as Nombre, `fechaN`, `edad`, `email`, `telefono`, `fotop`, `idpregunta`, `respuesta`, `religion`, `sexo` FROM `usuario` WHERE matricula = '"+mat+"'");
        datosUsr->exec();
        QSqlQuery staffExtr;
        staffExtr.prepare("SELECT puesto.nombre, areah.nombreArea, `turno` FROM `staff` INNER JOIN puesto on puesto.idpuesto = staff.idpuesto INNER JOIN areah on areah.idarea = staff.idArea WHERE idUser = '"+mat+"'");
        staffExtr.exec();
    }
    else if (tipoUsur == "paciente") {
        datosUsr->prepare("SELECT usuario.matricula, concat_ws(' ', usuario.nombre, usuario.appaterno, usuario.apmaterno) as Nombre, usuario.fechaN, usuario.edad, usuario.email, usuario.telefono, usuario.fotop, usuario.idpregunta, usuario.respuesta, usuario.religion, usuario.sexo FROM paciente INNER JOIN usuario on paciente.idUser = usuario.matricula WHERE paciente.idUser = '"+mat+"'");
        datosUsr->exec();
    }

    QString matri = datosUsr->value(0).toString();
    QString nombre = datosUsr->value(1).toString();
    QString mail = datosUsr->value(2).toString();

    QLabel* mat = new QLabel(this);
    mat->setMinimumSize(200, 40);
    mat->setMaximumSize(200, 40);
    mat->setText(matri);

    QLabel* nom = new QLabel(this);
    nom->setMinimumSize(200, 40);
    nom->setMaximumSize(200, 40);
    nom->setText(nombre);

    QLabel* correo = new QLabel(this);
    correo->setMinimumSize(200, 40);
    correo->setMaximumSize(200, 40);
    correo->setText(mail);

    QPushButton* btnModificar = new QPushButton();
    btnModificar->setMinimumSize(130, 35);
    btnModificar->setMaximumSize(130, 35);
    btnModificar->setText("Editar");
    btnModificar->setStyleSheet("");

    QPushButton* btnEliminar = new QPushButton();
    btnEliminar->setMinimumSize(130,35);
    btnEliminar->setMaximumSize(130,35);
    btnEliminar->setText("Eliminar");
    btnEliminar->setStyleSheet("");

}

EditarUsuario::~EditarUsuario()
{
    delete ui;
}
