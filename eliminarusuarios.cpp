#include "eliminarusuarios.h"
#include "ui_eliminarusuarios.h"
#include "editarusuario.h"

eliminarUsuarios::eliminarUsuarios(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::eliminarUsuarios)
{
    ui->setupUi(this);
}

eliminarUsuarios::~eliminarUsuarios()
{
    delete ui;
}

void clearLayou(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayou(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}


void eliminarUsuarios::eliminarDoc(QString matricula){

    qDebug()<<"eliminar doctor";

    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Estas seguro de eliminar al paciente?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));

    QMessageBox messageBox(QMessageBox::Warning,
    tr("Warning"), tr("no puedes eliminar el dotor tiene intervenciones pendientes."), QMessageBox::Yes);
messageBox.setButtonText(QMessageBox::Yes, tr("Aceptar"));

    QMessageBox messag(QMessageBox::Question,
    tr("Information"), tr("Doctor eliminado"), QMessageBox::Yes);
    messag.setButtonText(QMessageBox::Yes, tr("Aceptar"));

    QString preparada,consulta,fech,hor,tipo,cita,user1;
    preparada="Pendiente";
    QSqlQuery query,query1;
    QString fecha_inter;
    QDate fecha=QDate::currentDate();
    QString fecha_escritorio=fecha.toString("yyyy-MM-dd");

    if (message.exec() == QMessageBox::Yes ){
        //CUANDO EL DOCTOR TIENE INTERVENCIONES PENDIENTES
    query1.exec("select cq.idDoctor,e.fecha_llega,e.fecha_salida from CitasQuirofano as cq inner join Estancia as e on cq.idCita=e.idCitaQ  inner join doctor as d on cq.idDoctor=d.iddoctor where d.idUser='"+matricula+"'");
   while(query1.next()){
    fecha_inter=query1.value(2).toString();
    qDebug()<<"llll"<<fecha_inter;
    qDebug()<<"kkk"<<fecha_escritorio;

     }
      if(fecha_inter>fecha_escritorio){
          qDebug()<<"entre fecha";
        if (messageBox.exec() == QMessageBox::Yes){
         }

    }else {
      //CASO DONDE TIENE CITAS EL DOCTOR PENDIENTES
    query.exec("select usu.nombre,usu.appaterno,usu.apmaterno,usu.matricula,doc.estado,cit.estado as estadoCita,cit.preparada,cit.hora,cit.fecha,cit.matricula from usuario as usu inner join doctor as doc on usu.matricula=doc.idUser inner join  especialidad as esp on doc.idEspecialidad=esp.idEsp inner join cita as cit on doc.iddoctor=cit.doctor where usu.matricula='"+matricula+"'");
    while(query.next()){
         qDebug()<<"modificar:"<<query.value(6).toString();
        //IF PARA NOTIFICAR AL PACIENTE PARA DE QUE LE AN CANCELADO LAS CITAS.

        if(query.value(6).toString()=="Pendiente"){
        qDebug()<<"notificar al paciente y modificando citas que fueron aceptadas";
        //NOTIFICAMOS AL DOCTOR DE LAS CITAS QUE SE CANCELARON
        user1=query.value(9).toString();
        fech=query.value(8).toString();
        hor=query.value(7).toString();
        QString mensaj;
        mensaj="Se le informa que su Doctor ha cancelado la cita del dia: "+fech+" y con horario de: "+hor+" fue CANCELADA. Acuda o llame a nuestras oficinas para reagendar su cita";
        tipo="0";
        QString notificacion;
        notificacion="insert into notificacion(tipo,texto,UserP) values('"+tipo+"','"+mensaj+"','"+user1+"')";
        query.exec(notificacion);
        query.next();
     }
         /*PARA ACTUALIZAR EL PACIENTE ELIMINADO MODIFICAMOS LA CONTRASEÑA
          Y MODIFICAMOS CITAS QUE TENGA COMO ACEPTADAS EL DOCTOR PERO NO LAS A FINALIZADO*/
        //CAMBIAMOS LA CONTRASEÑA
          qDebug()<<"update";
         query.exec("update usuario set clave='0000' where matricula='"+matricula+"'");
         query.next();
         //CAMBIAMOS EL TIPO DE USUARIO PARA QUE NO LO RECONOZCA EL LOGIN Y LE EL NUMERO 5
         query.exec("update doctor set tipoUser='5' where idUser='"+matricula+"'");
         query.next();
         //CAMBIAMOS EL ESTADO Y LA PREPACRACION de sus citas
         query.exec("update cita as cit inner join doctor as doc on cit.doctor=doc.iddoctor set cit.estado='1',cit.preparada='Cancelada' where doc.idUser='"+matricula+"' and cit.estado='1' or cit.estado='0'");
         query.next();
         //CAMBIAMOS LA RESPUESTA PARA QUE NO PUEDAN RECUPERAR SU CONTRASEÑA
         query.exec("update  usuario set respuesta='Usuario Eliminado' where matricula='"+matricula+"'");
         query.next();
         if (messag.exec() == QMessageBox::Yes ){
         clearLayou(ui->gridLayout_eliminar);
         on_radioButton_doc_clicked();
         }
             }
    //CUANDO EL COTOR NO TIENE CITAS PENDIENTES
   query.exec("update usuario set clave='0000' where matricula='"+matricula+"'");
   query.next();
   //CAMBIAMOS EL TIPO DE USUARIO PARA QUE NO LO RECONOZCA EL LOGIN Y LE EL NUMERO 5
   query.exec("update doctor set tipoUser='5' where idUser='"+matricula+"'");
   query.next();
   //CAMBIAMOS LA RESPUESTA PARA QUE NO PUEDAN RECUPERAR SU CONTRASEÑA
   query.exec("update  usuario set respuesta='Usuario Eliminado' where matricula='"+matricula+"'");
   query.next();
   if (messag.exec() == QMessageBox::Yes ){
   clearLayou(ui->gridLayout_eliminar);
   on_radioButton_doc_clicked();
            }
        }
    }
 }



//METODO QUE SIRVE PARA DAR DE BAJA UN STAFF DEL SISTEMA
void eliminarUsuarios::eliminarStaff(QString matricula){

    qDebug()<<"eliminar staff"<<matricula;
    QSqlQuery query;
     clearLayou(ui->gridLayout_eliminar);
     QMessageBox message(QMessageBox::Question,
     tr("Information"), tr("¿Estas seguro de eliminar al staff?"), QMessageBox::Yes | QMessageBox::No);
     message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
     message.setButtonText(QMessageBox::No, tr("Cancelar"));


     QMessageBox messag(QMessageBox::Question,
     tr("Information"), tr("Staff eliminado"), QMessageBox::Yes);
     messag.setButtonText(QMessageBox::Yes, tr("Aceptar"));

     if (message.exec() == QMessageBox::Yes ){
    query.exec("update staff set estado='2',idArea='0',tipoUser='5' where idUser='"+matricula+"'");
    query.next();
    query.exec("update usuario set clave='0000' where matricula='"+matricula+"'");
    query.next();
    //CAMBIAMOS LA RESPUESTA PARA QUE NO PUEDAN RECUPERAR SU CONTRASEÑA
    query.exec("update  usuario set respuesta='Usuario Eliminado' where matricula='"+matricula+"'");
    query.next();
    if (messag.exec() == QMessageBox::Yes ){
    clearLayou(ui->gridLayout_eliminar);
      on_radioButton_staff_clicked();
    }
     }
}

//METODO QUE SIRVE PARA DAR DE BAJA UN PACIENTE DEL SISTEMA
void eliminarUsuarios::eliminarPaciente(QString matricula){

    QMessageBox message(QMessageBox::Question,
    tr("Information"), tr("¿Estas seguro de eliminar al paciente?"), QMessageBox::Yes | QMessageBox::No);
    message.setButtonText(QMessageBox::Yes, tr("Aceptar"));
    message.setButtonText(QMessageBox::No, tr("Cancelar"));


    QMessageBox messag(QMessageBox::Question,
    tr("Information"), tr("Paciente eliminado"), QMessageBox::Yes);
    messag.setButtonText(QMessageBox::Yes, tr("Aceptar"));

    qDebug()<<"eliminar Paciente";
    QString preparada,consulta,fech,hor,tipo,cita,user1,final;
    preparada="Pendiente";
    QSqlQuery query,query1;
    QString fecha_inter;
    QDate fecha=QDate::currentDate();
    QString fecha_escritorio=fecha.toString("yyyy-MM-dd");
    if (message.exec() == QMessageBox::Yes ){

        query1.exec("select cq.fechaCita,cq.idDoctor,cq.estado,p.idUser,cq.horaInicio from CitasQuirofano as cq inner join paciente as p on cq.idPaciente=p.idpaciente where p.idUser='"+matricula+"'");
       while(query1.next()){
          if(query1.value(2).toString()=="Pendiente" && query1.value(0).toString()>fecha_escritorio){
              qDebug()<<query1.value(0).toString()<<"ddddddddddddd"<<fecha_escritorio;
              qDebug()<<"notificar al Doctor y modificando intervenciones que fueron aceptadas";
              //NOTIFICAMOS AL DOCTOR DE LAS itervenciones QUE SE CANCELARON
              user1=query1.value(1).toString();
              fech=query1.value(0).toString();
              hor=query1.value(4).toString();
              QString mensaj;
              mensaj="Se le informa que su paciente ha cancelado la intervenvion del dia: "+fech+" y con horario de: "+hor+" fue CANCELADA.";
              tipo="0";
              QString notificacion;
              notificacion="insert into notificacion(tipo,texto,UserP) values('"+tipo+"','"+mensaj+"','"+user1+"')";
              query1.exec(notificacion);
              query1.next();
              query1.exec("update CitasQuirofano as cq inner join paciente as p on cq.idPaciente=p.idpaciente set cq.estado='Cancelada' where p.idUser='"+matricula+"' and cq.estado='Pendiente'");
              query1.next();
          }
       }
       //funciona correctamente probado
         query.exec("select *from cita where matricula='"+matricula+"'");
        while(query.next()){
        //IF PARA NOTIFICAR AL DOCTOR PARA DE QUE LE AN CANCELADO LAS CITAS.
        if(query.value(8).toString()=="Pendiente"){
        qDebug()<<"notificar al Doctor y modificando citas que fueron aceptadas";
        //NOTIFICAMOS AL DOCTOR DE LAS CITAS QUE SE CANCELARON
        user1=query.value(4).toString();
        fech=query.value(2).toString();
        hor=query.value(3).toString();
        QString mensaj;
        mensaj="Se le informa que su paciente ha cancelado la cita del dia: "+fech+" y con horario de: "+hor+" fue CANCELADA.";
        tipo="0";
        QString notificacion;
        notificacion="insert into notificacion(tipo,texto,UserP) values('"+tipo+"','"+mensaj+"','"+user1+"')";
        query.exec(notificacion);
        query.next();
        }

        /*PARA ACTUALIZAR EL PACIENTE ELIMINADO MODIFICAMOS LA CONTRASEÑA
          Y MODIFICAMOS CITAS QUE TENGA COMO ACEPTADAS EL DOCTOR PERO NO LAS A FINALIZADO*/

         query.exec("update usuario set clave='0000' where matricula='"+matricula+"'");
         query.next();
         query.exec("update cita set estado='2', preparada='Paciente eliminado' where matricula='"+matricula+"'");
         query.next();
         //CAMBIAMOS LA RESPUESTA PARA QUE NO PUEDAN RECUPERAR SU CONTRASEÑA
         query.exec("update  usuario set respuesta='Usuario Eliminado' where matricula='"+matricula+"'");
         query.next();

         //liberar el dia de la consulta->>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

        //ELIMINAMOS USUARIOS CON CITAS QUE AUN EL DOCTOR NO A ACEPATDO NI RECHAZADO
        if(query.value(8).toString()=="Pendiente"){//Correcto
            qDebug()<<"eliminadno citas que no ha aceptado el doctor aun";
            while(query.next()){
            qDebug()<<"eliminando citas que no ha aceptado al doctor";
            consulta="delete from cita where matricula='"+matricula+"' and preparada='"+preparada+"' and estado='0'";
            query.exec(consulta);
            query.next();

            }

      }

    }
    /*PARA ACTUALIZAR EL PACIENTE ELIMINADO MODIFICAMOS LA CONTRASEÑA
          Y MODIFICAMOS CITAS QUE TENGA COMO ACEPTADAS EL DOCTOR PERO NO LAS A FINALIZADO*/
      //probado y funcionando
         query.exec("update usuario set clave='0000' where matricula='"+matricula+"'");
         query.next();
         //CAMBIAMOS LA RESPUESTA PARA QUE NO PUEDAN RECUPERAR SU CONTRASEÑA
         query.exec("update  usuario set respuesta='Usuario Eliminado' where matricula='"+matricula+"'");
         query.next();
    if (messag.exec() == QMessageBox::Yes ){
        clearLayou(ui->gridLayout_eliminar);
        on_radioButton_pac_clicked();
        }
      }

}

void eliminarUsuarios::ModificarUsuario(QString matri, QString tipoUsr){
    qDebug()<<"modificar";
    EditarUsuario* editarVentana = new EditarUsuario(matri, tipoUsr);
    editarVentana->show();

    clearLayou(ui->gridLayout_eliminar);

}


void eliminarUsuarios::on_radioButton_doc_clicked()
{
    qDebug()<<"entre: doctor";
    clearLayou(ui->gridLayout_eliminar);
    int cont=0;
    clearLayou(ui->gridLayout_eliminar);
    QString consultaDoc,consultaStaff,nombre,espec,espera,matricula,useest1;
    QSqlQuery queryDoc,queryStaff,userEst;
    consultaDoc="select *from Doctores";
    queryDoc.exec(consultaDoc);
            while(queryDoc.next())
            {
                if(queryDoc.value(5).toString()=="0")
                {
                nombre=queryDoc.value(0).toString()+" "+queryDoc.value(1).toString()+" "+queryDoc.value(2).toString();
                espec=queryDoc.value(4).toString();
                espera="en espera";

                matricula=queryDoc.value(3).toString();

                QPushButton *b=new QPushButton();
                QPushButton *p=new QPushButton();
                b->setText("Eliminar Doctor");
                p->setText("Modificar Doctor");
                QLabel *l=new QLabel;
                l->setText(nombre);
                connect(b,&QPushButton::clicked,[=](){emit eliminarDoc(matricula);});
                connect(p,&QPushButton::clicked,[=](){emit ModificarUsuario(matricula, "doctor");});
                QLabel *espacio=new QLabel();
                QLabel *esp=new QLabel();
                esp->setText(espec);
                QLabel *estado=new QLabel();
                estado->setText(espera);
                ui->gridLayout_eliminar->addWidget(b,cont,0,1,1);
                 ui->gridLayout_eliminar->addWidget(p,cont,1,1,1);
                ui->gridLayout_eliminar->addWidget(l,cont,2,1,1);
                ui->gridLayout_eliminar->addWidget(esp,cont,3,1,1);
                ui->gridLayout_eliminar->addWidget(estado,cont,4,1,1);
                cont++;
                }
            }

}


void eliminarUsuarios::on_radioButton_staff_clicked()
{
    qDebug()<<"entre: staff";
    clearLayou(ui->gridLayout_eliminar);
    int cont=0;
    clearLayou(ui->gridLayout_eliminar);
    QString consultaDoc,consultaStaff,nombre,espec,espera,matricula,useest1;
    QSqlQuery queryStaff,userEst;
    consultaDoc="select * from  Staffs";
    queryStaff.exec(consultaDoc);
    while(queryStaff.next())
    {
        if(queryStaff.value(5).toString()=="1")
        {
            nombre=queryStaff.value(0).toString()+" "+queryStaff.value(1).toString()+" "+queryStaff.value(2).toString();
            espec=queryStaff.value(4).toString();
            espera="en espera";

            matricula=queryStaff.value(3).toString();

            QPushButton *b=new QPushButton();
            QPushButton *p=new QPushButton();
            b->setText(" Eliminar Staff ");
            p->setText("Modificar Staff");
            QLabel *l=new QLabel;
            l->setText(nombre);
            connect(b,&QPushButton::clicked,[=](){emit eliminarStaff(matricula);});
            connect(p,&QPushButton::clicked,[=](){emit ModificarUsuario(matricula, "staff");});
            QLabel *espacio=new QLabel();
            QLabel *esp=new QLabel();
            esp->setText(espec);
            QLabel *estado=new QLabel();
            estado->setText(espera);
            ui->gridLayout_eliminar->addWidget(b,cont,0,1,1);
            ui->gridLayout_eliminar->addWidget(p,cont,1,1,1);
            ui->gridLayout_eliminar->addWidget(l,cont,2,1,1);
            ui->gridLayout_eliminar->addWidget(esp,cont,3,1,1);
            cont++;
        }
    }

}

void eliminarUsuarios::on_radioButton_pac_clicked()
{
    qDebug()<<"entre: paciente";
    int cont=0;
    clearLayou(ui->gridLayout_eliminar);
    QString consultaDoc,consultaStaff,nombre,espec,espera,matricula,useest1;
    QSqlQuery queryPac;
    consultaDoc="select * from pacientes";
    queryPac.exec(consultaDoc);
    while(queryPac.next())
    {
            nombre=queryPac.value(0).toString()+" "+queryPac.value(1).toString()+" "+queryPac.value(2).toString();
            matricula=queryPac.value(3).toString();

            QPushButton *b=new QPushButton();
            QPushButton *p=new QPushButton();
            b->setText("Eliminar Paciente");
            p->setText("Modificar Paciente");
            QLabel *l=new QLabel;
            l->setText(nombre);

            connect(b,&QPushButton::clicked,[=](){emit eliminarPaciente(matricula);});
            connect(p,&QPushButton::clicked,[=](){emit ModificarUsuario(matricula, "paciente");});
            ui->gridLayout_eliminar->addWidget(b,cont,0,1,1);
            ui->gridLayout_eliminar->addWidget(p,cont,1,1,1);
            ui->gridLayout_eliminar->addWidget(l,cont,2,1,1);
            cont++;
        }
}
