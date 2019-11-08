#include "ordenarintervencion.h"
#include "ui_ordenarintervencion.h"

#include <QString>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QSignalMapper>

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
    //int anchoTabla = ui->tablaArticulos->width();
    ui->tablaArticulos->verticalHeader()->hide();
    ui->tablaArticulos->setColumnWidth(0,270);
    ui->tablaArticulos->setColumnWidth(1,92);
    ui->tablaArticulos->setColumnWidth(2,92);
    ui->tablaArticulos->setColumnWidth(3,59);
    ui->tablaArticulos->setColumnWidth(4,32);

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

void OrdenarIntervencion::on_btnAgregarArticulo_clicked()
{
    QString nombreItem = ui->lineServ->text();
    QString costo = ui->lineCosto->text();
    QString cantidad = ui->lineCantidad->text();
    if(nombreItem == "" || costo == ""){
        QMessageBox messageBox(QMessageBox::Warning,tr("Warning"), tr("Ingrese un artículo o servicio para continuar."), QMessageBox::Yes);
                 messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                 messageBox.exec();
    }
    else if (cantidad == "") {
        QMessageBox messageBox(QMessageBox::Warning,tr("Warning"), tr("Ingrese la cantidad deseada."), QMessageBox::Yes);
                 messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));
                 messageBox.exec();
    }
    else{
        double costoInt = costo.toDouble();
        double cantidadDoub = cantidad.toDouble();
        double total = costoInt * cantidadDoub;

        QString Total = "$ " + QString::number(total);
        QTableWidgetItem *nombre = new QTableWidgetItem(nombreItem);

        QTableWidgetItem *numero = new QTableWidgetItem(cantidad);
        numero->setTextAlignment(Qt::AlignHCenter);
        numero->setTextAlignment(Qt::AlignVCenter);

        QTableWidgetItem *precioTotal = new QTableWidgetItem(Total);
        precioTotal->setTextAlignment(Qt::AlignHCenter);
        precioTotal->setTextAlignment(Qt::AlignVCenter);

        QWidget* pWidget = new QWidget();

        QPushButton* btnEliminar = new QPushButton();
        QIcon icon(":/imgs/x-button.png");
        btnEliminar->setIcon(icon);
        btnEliminar->setIconSize(QSize(30,30));
        btnEliminar->show();
        btnEliminar->setMinimumWidth(30);
        btnEliminar->setMinimumHeight(30);
        btnEliminar->setMaximumWidth(30);
        btnEliminar->setMaximumHeight(30);
        btnEliminar->setStyleSheet("background-color: transparent; border:none;");

        QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
        pLayout->addWidget(btnEliminar);
        pLayout->setAlignment(Qt::AlignCenter);
        pLayout->setContentsMargins(0, 0, 0, 0);
        pWidget->setLayout(pLayout);

        QWidget* vacio = new QWidget();
        QLabel* vacia = new QLabel();
        QHBoxLayout* vacioLayout = new QHBoxLayout(vacio);
        vacioLayout->addWidget(vacia);
        vacioLayout->setAlignment(Qt::AlignCenter);
        vacioLayout->setContentsMargins(0, 0, 0, 0);
        vacio->setLayout(vacioLayout);

        ui->tablaArticulos->insertRow(ui->tablaArticulos->rowCount());
        int fila = ui->tablaArticulos->rowCount()-1;
        ui->tablaArticulos->setItem(fila, 0, nombre);
        ui->tablaArticulos->setItem(fila, 1, numero);
        ui->tablaArticulos->setItem(fila, 2, precioTotal);
        ui->tablaArticulos->setCellWidget(fila, 3, vacio);
        ui->tablaArticulos->setCellWidget(fila, 4, btnEliminar);

        connect(btnEliminar, &QPushButton::clicked, this, &OrdenarIntervencion::eliminarFila);

        ui->lineServ->clear();
        ui->lineCosto->clear();
        ui->lineCantidad->setText("1");
    }

}

void OrdenarIntervencion::eliminarFila()
{
    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Desea eliminar este artículo?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));
    if (message.exec() == QMessageBox::Yes){
        QWidget *w = qobject_cast<QWidget *>(sender()->parent());
        if(w){
            int row = ui->tablaArticulos->indexAt(w->pos()).row();
            ui->tablaArticulos->removeRow(row);
            ui->tablaArticulos->setCurrentCell(0, 0);
        }
    }
    else{
        message.close();
    }
}

void OrdenarIntervencion::actualizaPrecios(){

}
