#include "eliminarusuarios.h"
#include "ui_eliminarusuarios.h"

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
    QString preparada,consulta,fech,hor,tipo,cita,user1;
    preparada="Pendiente";
    QSqlQuery query;

    query.exec("select usu.nombre,usu.appaterno,usu.apmaterno,usu.matricula,doc.estado,cit.estado as estadoCita,cit.preparada from usuario as usu inner join doctor as doc on usu.matricula=doc.idUser inner join  especialidad as esp on doc.idEspecialidad=esp.idEsp inner join cita as cit on doc.iddoctor=cit.doctor where usu.matricula='"+matricula+"'");
    while(query.next()){
         qDebug()<<"modificar:"<<query.value(6).toString();
        //IF PARA NOTIFICAR AL PACIENTE PARA DE QUE LE AN CANCELADO LAS CITAS.

        if(query.value(6).toString()=="Pendiente"){

        qDebug()<<"notificar al paciente y modificando citas que fueron aceptadas";

        //NOTIFICAMOS AL DOCTOR DE LAS CITAS QUE SE CANCELARON
        //falta en el select principal agregra la fecha, hora
        user1=query.value(4).toString();
        fech=query.value(2).toString();
        hor=query.value(3).toString();
        QString mensaj;
        mensaj="Se le informa que su ------doctor ha cancelado la cita del dia: "+fech+" y con horario de: "+hor+" fue CANCELADA. Acuda o llame a nuestras oficinas para reagendar su cita";
        tipo="1";
        QString notificacion;
        notificacion="insert into notificacion(tipo,texto,UserP) values('"+tipo+"','"+mensaj+"','"+user1+"')";
        query.exec(notificacion);
        query.next();

        }

         /*PARA ACTUALIZAR EL PACIENTE ELIMINADO MODIFICAMOS LA CONTRASEÑA
          Y MODIFICAMOS CITAS QUE TENGA COMO ACEPTADAS EL DOCTOR PERO NO LAS A FINALIZADO*/

         query.exec("update usuario set clave='0000' where matricula='"+matricula+"'");
         query.next();
         query.exec("update doctor set tipoUser='5' where idUser='"+matricula+"'");
         query.next();
         query.exec("update cita as cit inner join doctor as doc on cit.doctor=doc.iddoctor set cit.preparada='Cancelada' where doc.idUser'"+matricula+"'");
         query.next();

    }
     clearLayou(ui->gridLayout_eliminar);
    on_radioButton_doc_clicked();
    //datos que debo modificar para enviar una cita con el admin para reasignar
    //estado = 1 AND preparada = 'Cancelada'


}



//METODO QUE SIRVE PARA DAR DE BAJA UN STAFF DEL SISTEMA
void eliminarUsuarios::eliminarStaff(QString matricula){

    qDebug()<<"eliminar staff"<<matricula;
    QSqlQuery query;
     clearLayou(ui->gridLayout_eliminar);
    query.exec("update staff set estado='2',idArea='0',tipoUser='5' where idUser='"+matricula+"'");
    query.next();
    query.exec("update usuario set clave='0000' where matricula='"+matricula+"'");
    query.next();
}
//METODO QUE SIRVE PARA DAR DE BAJA UN PACIENTE DEL SISTEMA
void eliminarUsuarios::eliminarPaciente(QString matricula){

    qDebug()<<"eliminar Paciente";
    QString preparada,consulta,fech,hor,tipo,cita,user1;
    preparada="Pendiente";
    QSqlQuery query;
 clearLayou(ui->gridLayout_eliminar);
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
    on_radioButton_doc_clicked();
}




void eliminarUsuarios::ModificarUsuario(QString){

    qDebug()<<"modificar";

    clearLayou(ui->gridLayout_eliminar);

}


void eliminarUsuarios::on_radioButton_doc_clicked()
{
    qDebug()<<"entre:";
    int cont=0;
    clearLayou(ui->gridLayout_eliminar);
    QString consultaDoc,consultaStaff,nombre,espec,espera,matricula,useest1;
    QSqlQuery queryDoc,queryStaff,userEst;
    consultaDoc="select *from Doctores";
    queryDoc.exec(consultaDoc);
            while(queryDoc.next())
            {
                if(queryDoc.value(5).toString()=="1")
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
                connect(p,&QPushButton::clicked,[=](){emit ModificarUsuario(matricula);});
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
    qDebug()<<"entre:";
    int cont=0;
    clearLayou(ui->gridLayout_eliminar);
    QString consultaDoc,consultaStaff,nombre,espec,espera,matricula,useest1;
    QSqlQuery queryStaff,userEst;
    consultaDoc="select *from  Staffs";
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
            connect(p,&QPushButton::clicked,[=](){emit ModificarUsuario(matricula);});
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
    consultaDoc="select *from pacientes";
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
            connect(p,&QPushButton::clicked,[=](){emit ModificarUsuario(matricula);});
            ui->gridLayout_eliminar->addWidget(b,cont,0,1,1);
            ui->gridLayout_eliminar->addWidget(p,cont,1,1,1);
            ui->gridLayout_eliminar->addWidget(l,cont,2,1,1);
            cont++;
        }
}

