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
    clearLay(ui->lay_usu);
    //Llenamos los datos
    ui->detalles->setStyleSheet("#color{background-color:"+datos.value("colorEmergencia").toString()+"}");
    ui->paciente->setText(datos.value("nombre_pacinete").toString());
    ui->fecha->setText(datos.value("fecha").toString());
    ui->hora->setText(datos.value("hora").toString());
    ui->formaLlegada->setPlainText(datos.value("forma_llegada").toString());
    ui->causas->setPlainText(datos.value("Causas").toString());
    ui->signosVitales->setPlainText(datos.value("signos_vitales").toString());

    //Para poner el doctor que hay que buscarlo
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
    if( !buscar->exec("SELECT matricula FROM usuario,paciente WHERE "
                 "nombre = '"+nombreC[0]+"' "
                 "AND appaterno = '"+nombreC[1]+"' "
                 "AND apmaterno = '"+nombreC[2]+"' "
                 "AND usuario.matricula=paciente.idUser") ) qDebug()<<buscar->lastError().text();
    //Si hay coincidencias configurar el boton
    if(!buscar->next()){
        ui->pb_expediente->setText(" Completar expediente ");
        connect(ui->pb_expediente,&QPushButton::clicked,[=](){emit nuevoExp();});
        connect(ui->pb_agregar,&QPushButton::clicked,[=](){emit on_pb_agregar_clicked(datos.value(0).toString());});
        ui->mensaje->setText("Este paciente no está registrado en el sistema");
    }
    else{
        ui->pb_expediente->setText(" Agregar a expediente ");
        connect(ui->pb_expediente,&QPushButton::clicked,[=](){emit agregarExistente(datos.value("idEmergencia").toString());});
        ui->mensaje->setText("Puede que ya exista un paciente registrado con este nombre");
    }

    delete(buscar);
    delete(doc);
    ui->sw_urgencias->setCurrentIndex(1);
}

//Regresar
void agregarUrgencia::on_pushButton_clicked()
{
    clearLay(ui->lay_usu);
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
    delete(compa);
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
                                delete(fechaActual);
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
                    delete(datosUrg);
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

void agregarUrgencia::agregarExistente(QString idE){
    //Ciclo para agregar los datos
    QSqlQuery *search = new QSqlQuery;
    QStringList nombreC = ui->paciente->text().split(" ");
    if( !search->exec("SELECT matricula, CONCAT(nombre,' ',appaterno,' ',apmaterno) as nombre, fechaN,sexo,fotop FROM usuario,paciente WHERE "
                     "nombre = '"+nombreC[0]+"' "
                     "AND appaterno = '"+nombreC[1]+"' "
                     "AND apmaterno = '"+nombreC[2]+"' "
                     "AND usuario.matricula=paciente.idUser") )
        qDebug()<<search->lastError().text();
    QString nombre, fechaN, sexo, matri;
    QVariant foto;
    QPixmap pix;
    QLabel *l;
    QHBoxLayout *hlay = new QHBoxLayout;
    QFrame *framecito;
    while(search->next()){
        nombre=" "+search->value("nombre").toString()+" ";
        l =new QLabel;
        l->setText(nombre);
        l->setAlignment(Qt::AlignCenter);
        hlay->addWidget(l,Qt::AlignCenter);

        l=new QLabel;
        fechaN=" "+search->value("fechaN").toString()+" ";
        l->setText(fechaN);
        l->setAlignment(Qt::AlignCenter);
        hlay->addWidget(l,Qt::AlignCenter);

        l=new QLabel;
        sexo=" "+search->value("sexo").toString()+" ";
        l->setText(sexo);
        l->setAlignment(Qt::AlignCenter);
        hlay->addWidget(l,Qt::AlignCenter);

        l=new QLabel;
        sexo=search->value("fotop").toString();
        l->setFixedSize(50,50);
        pix.loadFromData(search->value("fotop").toByteArray());
        l->setPixmap(pix.scaled(50,50,Qt::KeepAspectRatioByExpanding));
        l->setAlignment(Qt::AlignLeft);
        hlay->addWidget(l,Qt::AlignLeft);

        QPushButton *pb = new QPushButton;
        pb->setText("Usar perfil");
        pb->setFixedSize(QSize(70,25));
        matri=search->value("matricula").toString();
        connect(pb,&QPushButton::clicked,[=](){emit llenarHistorial(matri,idE);});
        qDebug()<<"MAtri: "+matri;
        hlay->addWidget(pb,Qt::AlignCenter);

        hlay->setAlignment(Qt::AlignCenter);
        hlay->setSpacing(0);
        framecito = new QFrame;
        framecito->setLayout(hlay);
        framecito->setStyleSheet("QFrame{background-color: pink;} "
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
        ui->lay_usu->addWidget(framecito);
    }
    //Cuando termine hay que agregar una barra espaciadora para empujar el contenido
    QSpacerItem *barraVertical= new QSpacerItem(10,10,QSizePolicy::Ignored,QSizePolicy::Expanding);
    ui->lay_usu->addSpacerItem(barraVertical);

    //Movemos a la lista de posibles usuarios
    ui->sw_urgencias->setCurrentIndex(5);
}

void agregarUrgencia::llenarHistorial(QString id, QString idE){
    QMessageBox::StandardButton preg=QMessageBox::question(this,"Usar perfil","¿Está seguro usar este perfil?");
    if(preg == QMessageBox::Yes){
        //Llenamos los datos
        //Obetenmos id del paciente para buscar en historial
        QString idPac;
        QSqlQuery *pac = new QSqlQuery, *histo= new QSqlQuery;
        pac->exec("SELECT idpaciente FROM paciente WHERE idUser="+id);
        qDebug()<<"Pac: "+id;
        pac->next();
        idPac=pac->value("idpaciente").toString();
        //Llenamos el historial
        histo->exec("SELECT * FROM historialPaciente WHERE idPaciente="+idPac);
        if(!histo->next()){
            //Si no tenia un historial
            histo->exec("INSERT INTO historialPaciente(idPaciente) value("+idPac+")");
            histo->exec("SELECT * FROM historialPaciente WHERE idPaciente="+idPac);
            histo->next();
        }
        // si si tenia un hisorial ya cargamos sus datos a la ui

        ui->le_estatura->setText(histo->value(1).toString());
        ui->le_peso->setText(histo->value(2).toString());
        ui->de_ultimaVacuna->setDate(histo->value(3).toDate());
        ui->te_alergias->setText(histo->value(4).toString());
        ui->te_accidentes->setText(histo->value(5).toString());
        ui->te_enfermedades->setText(histo->value(6).toString());
        ui->te_cirugias->setText(histo->value(7).toString());
        ui->te_hospi->setText(histo->value(8).toString());
        ui->te_trabajos->setText(histo->value(9).toString());
        ui->te_habitos->setText(histo->value(10).toString());
        ui->te_frecAlco->setText(histo->value(11).toString());
        ui->te_frecFuma->setText(histo->value(12).toString());
        ui->te_enfeFami->setText(histo->value(13).toString());
        delete(pac);
        delete(histo);
        histo->~QSqlQuery();
        //Movemos a la pagina del historial
        connect(ui->pb_llenarHistorial,&QPushButton::clicked,[=](){emit on_pb_llenarHistorial_clicked(idPac,idE);});
        ui->sw_urgencias->setCurrentIndex(4);
    }
}

//Cuando le da click a guargar historial
void agregarUrgencia::on_pb_llenarHistorial_clicked(QString idPac, QString idE)
{
    QSqlQuery *llenar= new QSqlQuery;
    if( !llenar->exec("UPDATE historialPaciente SET "
                             " estatura="+ui->le_estatura->text()+
                             ", peso="+ui->le_peso->text()+
                             ", fechaUltimaVacuna='"+ui->de_ultimaVacuna->date().toString("yyyy-MM-dd")+
                             "', alergias='"+ui->te_alergias->toPlainText()+
                             "', accidente='"+ui->te_accidentes->toPlainText()+
                             "', enfermedad='"+ui->te_enfermedades->toPlainText()+
                             "', cirugias='"+ui->te_cirugias->toPlainText()+
                             "', hospitalizaciones='"+ui->te_hospi->toPlainText()+
                             "', trabajos='"+ui->te_trabajos->toPlainText()+
                             "', habitos='"+ui->te_habitos->toPlainText()+
                             "', frecuenciaAlcohol='"+ui->te_frecAlco->toPlainText()+
                             "', frecuenciaCigarro='"+ui->te_frecFuma->toPlainText()+
                             "', enfermedadesFamilia='"+ui->te_enfeFami->toPlainText()+
                             "' WHERE idPaciente="+idPac ) ){
        qDebug()<<llenar->lastError().text();
        QMessageBox::warning(this,"Error","No se pudieron guardar los datos");
    }
    else{
        if( llenar->exec("UPDATE urgencias SET estado=1 WHERE idEmergencia="+idE) ){
            QMessageBox::information(this,"Correcto","Datos ingresados correctamente.");
            buscarUrgencias();
            ui->sw_urgencias->setCurrentIndex(0);
        }
        else{
            QMessageBox::warning(this,"Error","No se pudo guardar los datos.");
        }
    }
    delete(llenar);
}

//Boton para recargar
void agregarUrgencia::on_pushButton_2_clicked()
{
    buscarUrgencias();
}

// un boton para regresar
void agregarUrgencia::on_pushButton_4_clicked()
{
    clearLay(ui->lay_usu);
    ui->sw_urgencias->setCurrentIndex(1);
}

