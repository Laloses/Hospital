#include "agregarUrgencia.h"
#include "ui_agregarUrgencia.h"

agregarUrgencia::agregarUrgencia(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::agregarUrgencia)
{
    ui->setupUi(this);
    this->setWindowTitle("Urgencias");
    this->setFixedSize(640,480);
    urgencias=new QSqlQuery;
    QSqlDatabase::database();
    //Buscamos todas las urgencias por fecha
    ui->sw_urgencias->setCurrentIndex(0);
    buscarUrgencias();
}

agregarUrgencia::~agregarUrgencia()
{
    delete ui;
}

void clearLay(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLay(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void agregarUrgencia::buscarUrgencias(){
    clearLay(ui->listaUrgencias);
    QList<QSqlRecord> datos;
    QString consulta="SELECT * "
                     "FROM urgencias WHERE estado = 0 "
                     "ORDER BY fecha,hora ";
    urgencias->exec(consulta);
    while(urgencias->next()){
        datos.append(urgencias->record());
    }
    crearBotones(datos);
}

void agregarUrgencia::crearBotones(QList<QSqlRecord> datos){
    QHBoxLayout *hlay = new QHBoxLayout;
    QLabel *l;
    QFrame *color, *framecito;
    int i=0, tam=datos.size();
    QString id,forma,fechaHora;
    //Hacemos un ciclo para cada urgencia
    for(i=0; i<tam; i++){
        //Los guardamos en un layaout horizontal
        hlay = new QHBoxLayout;
        framecito = new QFrame;

        l =new QLabel;
        id=datos.at(i).value("idEmergencia").toString();
        l->setText(id);
        l->setAlignment(Qt::AlignCenter);
        hlay->addWidget(l,Qt::AlignCenter);

        color=new QFrame;
        color->setObjectName("color"+QString::number(i));
        color->setFixedSize(30,10);
        hlay->addWidget(color,Qt::AlignCenter);

        l=new QLabel;
        forma=datos.at(i).value("forma_llegada").toString();
        l->setText(forma);
        l->setAlignment(Qt::AlignCenter);
        hlay->addWidget(l,Qt::AlignCenter);

        l=new QLabel;
        fechaHora=datos.at(i).value("fecha").toString()+" "+datos.at(i).value("hora").toString();
        l->setText(fechaHora);
        l->setAlignment(Qt::AlignCenter);
        hlay->addWidget(l,Qt::AlignCenter);

        QPushButton *pb = new QPushButton;
        pb->setText("Detalles");
        pb->setFixedSize(QSize(70,25));
        connect(pb,&QPushButton::clicked,[=](){emit verDetalles(datos.at(i));});
        hlay->addWidget(pb,Qt::AlignCenter);

        hlay->setAlignment(Qt::AlignCenter);
        hlay->setSpacing(0);
        framecito->setLayout(hlay);
        framecito->setStyleSheet("QFrame{background-color: pink;} "
                                 "#color"+QString::number(i)+"{background-color:"+datos.at(i).value("colorEmergencia").toString()+"}"
                                 "QPushButton{"
                                 "border:solid 1px #5d80b6;"
                                 "border-radius:5px;"
                                 "background-color: #5d80b6;"
                                 "color: white;"
                                 "font: 11pt 'MS Shell Dlg 2';"
                                 "height:30px;}"
                                 "QPushButton:hover{"
                                 "padding-top:1px;"
                                 "background-color:black;}");
        ui->listaUrgencias->addWidget(framecito);
    }
    //Cuando termine hay que agregar una barra espaciadora para empujar el contenido
    QSpacerItem *barraVertical= new QSpacerItem(10,10,QSizePolicy::Ignored,QSizePolicy::Expanding);
    ui->listaUrgencias->addSpacerItem(barraVertical);
}

void agregarUrgencia::verDetalles(QSqlRecord datos){
    //Llenamos los datos
    ui->detalles->setStyleSheet("#color{background-color:"+datos.value("colorEmergencia").toString()+"}");
    ui->paciente->setText(datos.value("nombre_pacinete").toString());
    ui->fecha->setText(datos.value("fecha").toString());
    ui->hora->setText(datos.value("hora").toString());
    ui->formaLlegada->setPlainText(datos.value("forma_llegada").toString());
    ui->causas->setPlainText(datos.value("Causas").toString());
    ui->signosVitales->setPlainText(datos.value("signos_vitales").toString());

    //Para poner el docor que hay que buscarlo
    QSqlQuery *doc = new QSqlQuery;
    doc->exec("SELECT idUser FROM doctor WHERE iddoctor="+datos.value("idDoctor").toString());
    doc->next();
    doc->exec("SELECT CONCAT(nombre,' ',appaterno,' ',apmaterno) as nombre FROM usuario WHERE matricula="+doc->value("idUser").toString());
    doc->next();
    ui->doctor->setText(doc->value("nombre").toString());

    //Conectar boton para ver su acompañante
    connect(ui->pb_compa,&QPushButton::clicked,[=](){emit verCompa(datos.value("idEmergencia").toString());});

    //Verificamos si hay usuarios registrados con ese nombre para añadir un nuevo usuario o añadir al expediente
    QStringList nombreC = ui->paciente->text().split(" ");
    QSqlQuery *buscar= new QSqlQuery;
    if( !buscar->exec("SELECT matricula FROM usuario WHERE "
                 "nombre = '"+nombreC[0]+"' "
                 "AND appaterno = '"+nombreC[1]+"' "
                 "AND apmaterno = '"+nombreC[2]+"'") ) qDebug()<<buscar->lastError().text();
    //Si hay coincidencias configurar el boton
    if(!buscar->next()){
        ui->pb_expediente->setText(" Completar expediente ");
        connect(ui->pb_expediente,&QPushButton::clicked,[=](){emit nuevoExp();});
        connect(ui->pb_agregar,&QPushButton::clicked,[=](){emit on_pb_agregar_clicked(datos.value(0).toString());});
        ui->mensaje->setText("Este paciente no está registrado en el sistema");
    }
    else{
        ui->pb_expediente->setText(" Llenar urgencia ");
        connect(ui->pb_expediente,&QPushButton::clicked,[=](){emit agregarExistente(datos);});
        ui->mensaje->setText("Puede que ya exista un paciente registrado con este nombre");
    }

    ui->sw_urgencias->setCurrentIndex(1);
}

//Regresar
void agregarUrgencia::on_pushButton_clicked()
{
    ui->sw_urgencias->setCurrentIndex(0);
}

void agregarUrgencia::verCompa(QString id)
{
    //Podemos los datos
    QSqlQuery *compa = new QSqlQuery;
    compa->exec("SELECT * FROM acompanante WHERE idEmergencia="+id);
    compa->next();
    ui->nomCompa->setText(compa->value("nombre").toString());
    ui->parentesco->setText(compa->value("parentescos").toString());
    ui->dirCompa->setText(compa->value("direcion").toString());
    ui->telCompa->setText(compa->value("telefono").toString());

    //Para hacer chica la pantalla
    ui->detalles->setHidden(true);
    ui->todas->setHidden(true);
    this->setFixedSize(640,190);
    ui->sw_urgencias->setCurrentIndex(2);
}

void agregarUrgencia::on_regresarDetalles_clicked()
{
    //Ponemos el tamaño de la pantalla
    ui->detalles->setHidden(false);
    ui->todas->setHidden(false);
    this->setFixedSize(640,480);
    ui->sw_urgencias->setCurrentIndex(1);
}

void agregarUrgencia::nuevoExp(){
    QStringList nomC = ui->paciente->text().split(" ");
    //Llenamos los datos que ya tenemos
    ui->le_hora->setText(ui->hora->text());
    ui->lineEdit_nombre->setText(nomC[0]);
    if(nomC.size()>1) ui->lineEdit_apePaterno->setText(nomC[1]);
    if(nomC.size()>2) ui->lineEdit_apeMaterno->setText(nomC[2]);
    ui->formaLlegada_2->setPlainText(ui->formaLlegada->toPlainText());
    ui->signosVitales_2->setPlainText(ui->signosVitales->toPlainText());
    ui->causas_2->setPlainText(ui->causas->toPlainText());

    //Movemos a la pagina de datos del paciente
    ui->sw_urgencias->setCurrentIndex(3);
    ui->sw_nuevoExp->setCurrentIndex(0);
    //Lo hacemos más pequeño
    this->setFixedSize(640,180);
}

//Cuando ya llenó los datos del paciente
void agregarUrgencia::on_pb_siguiente_clicked()
{
    QString estiloBueno,estiloMalo;
    estiloBueno="border-top:none;border-bottom:1px solid #5d80b6;background:transparent;font: 12pt MS Shell Dlg 2;";
    estiloMalo="border:2px red; border-style:solid";

    if(!ui->lineEdit_nombre->text().isEmpty()){
        ui->lineEdit_nombre->setStyleSheet(estiloBueno);
        if(!ui->lineEdit_apePaterno->text().isEmpty()){
            ui->lineEdit_apePaterno->setStyleSheet(estiloBueno);
            if(!ui->lineEdit_apeMaterno->text().isEmpty()){
                ui->lineEdit_apeMaterno->setStyleSheet(estiloBueno);
                if(!ui->lineEdit_email->text().isEmpty()){
                    ui->lineEdit_email->setStyleSheet(estiloBueno);
                    if(!ui->lineEdit_telefono->text().isEmpty()){
                        ui->lineEdit_telefono->setStyleSheet(estiloBueno);
                        if(!ui->le_sexo->text().isEmpty()){
                            ui->le_sexo->setStyleSheet(estiloBueno);

                            //Cuando ya todo está bien
                            //Preguntamos
                            QMessageBox::StandardButton preg=QMessageBox::question(this,"Terminar registro","¿Está seguro de guardar sus datos?");
                            if(preg == QMessageBox::Yes){

                                registropaciente *reg=new registropaciente;
                                QSqlQuery *fechaActual=new QSqlQuery;
                                if( fechaActual->exec("SELECT YEAR(CURDATE())-YEAR('"+ui->dateEdit_fNacimiento->date().toString("yyyy-MM-dd")+"') + IF( DATE_FORMAT(CURDATE(),'%m-%d') > DATE_FORMAT('"+ui->dateEdit_fNacimiento->date().toString("yyyy-MM-dd")+"','%m-%d'),0, -1)") ){
                                    fechaActual->next();
                                }
                                QString imgRuta;
                                if(ui->le_sexo->text().toLower() == "femenino"){
                                    imgRuta = "C:/Iconos Perfiles/girl.png";
                                }
                                else{
                                    imgRuta = "C:/Iconos Perfiles/boy2.png";
                                }
                                if( reg->registroPaciente("lobohospital",
                                                          ui->lineEdit_nombre->text(),
                                                          ui->lineEdit_apePaterno->text(),
                                                          ui->lineEdit_apeMaterno->text(),
                                                          ui->dateEdit_fNacimiento->date().toString("yyyy-MM-dd"),
                                                          fechaActual->value(0).toString(),
                                                          ui->lineEdit_email->text(),
                                                          ui->lineEdit_telefono->text(),
                                                          imgRuta,
                                                          "1",
                                                          "lobohospital",
                                                          ui->le_sexo->text() ) == "0"){
                                    qDebug()<<"Error al agregar al usuario";
                                }
                                //Si si se inserto correctamente
                                else{
                                    idPac=reg->getIdPac();
                                    this->setFixedSize(640,480);
                                    //Nos movemos a la siguiente página
                                        ui->sw_nuevoExp->setCurrentIndex(1);
                                        ui->le_nomPac->setText(ui->lineEdit_nombre->text()+" "+ui->lineEdit_apePaterno->text()+" "+ui->lineEdit_apeMaterno->text());
                                        ui->le_nomPac->setEnabled(false);
                                }
                            }
                        }
                        else{
                            ui->le_sexo->setStyleSheet(estiloMalo);
                        }
                    }
                    else{
                         ui->lineEdit_telefono->setStyleSheet(estiloMalo);
                    }
                }
                else{
                     ui->lineEdit_email->setStyleSheet(estiloMalo);
                }
            }
            else{
                 ui->lineEdit_apeMaterno->setStyleSheet(estiloMalo);
            }
        }
        else{
             ui->lineEdit_apePaterno->setStyleSheet(estiloMalo);
        }
    }
    else{
        ui->lineEdit_nombre->setStyleSheet(estiloMalo);
    }

}

//Si quiere regresar a la ventana anterior
void agregarUrgencia::on_pushButton_3_clicked()
{
    this->setFixedSize(640,480);
    ui->sw_urgencias->setCurrentIndex(1);
}

//Para agregar los datos al exp
void agregarUrgencia::on_pb_agregar_clicked(QString id)
{
    QString estiloBueno,estiloMalo;
    estiloBueno="border-top:none;border-bottom:1px solid #5d80b6;background:transparent;font: 12pt MS Shell Dlg 2;";
    estiloMalo="border:2px red; border-style:solid";
    if(!ui->diagnostico->toPlainText().isEmpty()){
        ui->lineEdit_nombre->setStyleSheet(estiloBueno);

        QMessageBox::StandardButton preg=QMessageBox::question(this,"Terminar registro","¿Está seguro de guardar sus datos?");
        if(preg == QMessageBox::Yes){

            QString consulta="INSERT INTO expedienteUrgencias(idPaciente,forma_llegada,Causas,signos_vitales,hora,fecha,diagnostico)  values(";
            consulta+=idPac+",'";
            consulta+=ui->formaLlegada_2->toPlainText()+"','";
            consulta+=ui->causas_2->toPlainText()+"','";
            consulta+=ui->signosVitales_2->toPlainText()+"','";
            consulta+=ui->le_hora->text()+"','";
            consulta+=ui->dateEdit->date().toString("yyyy-MM-dd")+"','";
            consulta+=ui->diagnostico->toPlainText()+"')";

            QSqlQuery *datosUrg = new QSqlQuery;
            if( datosUrg->exec(consulta) ){
                //Actualizamos
                consulta="UPDATE urgencias SET estado=1 WHERE idEmergencia="+id;
                if( datosUrg->exec(consulta) ){
                    //Si se guardan correctamente
                    QMessageBox::information(this,"Correcto","Datos guardados correctamente");
                    buscarUrgencias();
                    ui->sw_urgencias->setCurrentIndex(0);
                }
            }
            else{
                qDebug()<<datosUrg->lastError().text();
            }

        }
    }
    else{
        ui->lineEdit_nombre->setStyleSheet(estiloMalo);
    }

}

void agregarUrgencia::agregarExistente(QSqlRecord datos){

}

void agregarUrgencia::on_pushButton_2_clicked()
{
    buscarUrgencias();
}
