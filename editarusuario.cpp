#include "editarusuario.h"
#include "ui_editarusuario.h"

EditarUsuario::EditarUsuario(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditarUsuario)
{
    ui->setupUi(this);
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
    datosUsr->prepare("SELECT `matricula`, concat_ws(' ', `nombre`, `appaterno`, `apmaterno`) as Nombre, `email` FROM `usuario`");

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
