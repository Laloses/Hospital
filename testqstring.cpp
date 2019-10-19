#include <QtTest>
#include<QtTest/QtTest>
#include <QCoreApplication>
#include <QtSql/QSqlQuery>
#include <QTestEventList>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QtDebug>
#include <QtWidgets/QLineEdit>
//#include <QMainWindow>

// add necessary includes here

class prueba1 : public QObject
{
    Q_OBJECT

public:
    prueba1();
    ~prueba1();

private slots:

  //-----Sprint 2---------------
    //void dbTest();
    //void registrarPaciente();
    //void registrarStaff();
    //void registrarDoctor();
    //void loginTest();
   void loginPaciente();
    //void loginStaff();
    //void loginDoctor();
    //void loginAdmi();
    //void resgistrCliente();


};


prueba1::prueba1()
{

}

prueba1::~prueba1()
{

}


/*void prueba1::dbTest()
{
    QSqlDatabase mDatabase;
    mDatabase=QSqlDatabase::addDatabase("QMYSQL");
    mDatabase.setHostName("localhost");
    mDatabase.setDatabaseName("lobohospital");
    mDatabase.setPort(3306);
    mDatabase.setUserName("root");
    mDatabase.setPassword("");
    if(!mDatabase.open())
        qDebug ()<<"Base de Datos No Conectada";
    else
        qDebug ()<<"Base de Datos Conectada.";
    QVERIFY(mDatabase.isValid());
    QCOMPARE(mDatabase.isOpen(),true);
}*/

/*void prueba1::registrarPaciente(){
    QSqlDatabase mDatabase;
   mDatabase=QSqlDatabase::addDatabase("QMYSQL");
    mDatabase.setHostName("localhost");
    mDatabase.setDatabaseName("lobohospital");
    mDatabase.setPort(3306);
    mDatabase.setUserName("root");
    mDatabase.setPassword("");
    mDatabase.open();
     QLineEdit matricula,clave,nombre,appaterno,apmaterno,fechaN,edad,email,telefono,fotop,idpregunta,respuesta,idpaciente,idUser;
     QTest::keyClicks(&matricula,"20192399");
     QTest::keyClicks(&clave,"1234");
     QTest::keyClicks(&nombre,"Abraham");
     QTest::keyClicks(&appaterno,"rodriguez");
     QTest::keyClicks(&apmaterno,"alarcon");
     QTest::keyClicks(&fechaN,"1996-02-02");
     QTest::keyClicks(&edad,"23");
     QTest::keyClicks(&email,"a@gmail.com");
     QTest::keyClicks(&telefono,"1234567");
     QTest::keyClicks(&fotop,"C:/imagnes tips/agua.jpg");
     QTest::keyClicks(&idpregunta,"1");
     QTest::keyClicks(&respuesta,"pop");
     QSqlQuery insertar,insertarp;
      insertar.prepare("insert into usuario(matricula,clave,nombre,appaterno,apmaterno,fechaN,edad,email,telefono,fotop,idpregunta,respuesta)"
     "values('"+matricula.text()+"','"+clave.text()+"','"+nombre.text()+"','"+appaterno.text()+"','"+apmaterno.text()+"',"
   "'"+fechaN.text()+"','"+edad.text()+"','"+email.text()+"','"+telefono.text()+"',LOAD_FILE('"+fotop.text()+"'),"
       "'"+idpregunta.text()+"','"+respuesta.text()+"')");
      if(insertar.exec()){
          if(insertarp.prepare(("insert into paciente() value (NULL,'"+matricula.text()+"')"))){
        qDebug()<<"Paciente registrado";
        qDebug()<<"Paciente registrado en tabla paciente";
           }
           else{
               qDebug()<<"Paciente no registrado";
               qDebug()<<"Paciente no registrado en tabla paciente";
           }
      }
        mDatabase.close();
}*/

/*void prueba1::registrarStaff(){
    QSqlDatabase mDatabase;
   mDatabase=QSqlDatabase::addDatabase("QMYSQL");
    mDatabase.setHostName("localhost");
    mDatabase.setDatabaseName("lobohospital");
    mDatabase.setPort(3306);
    mDatabase.setUserName("root");
    mDatabase.setPassword("");
    mDatabase.open();
     QLineEdit matricula,clave,nombre,appaterno,apmaterno,fechaN,edad,email,telefono,fotop,idpregunta,respuesta,idPuesto,estado;
     QTest::keyClicks(&matricula,"20192349");
     QTest::keyClicks(&clave,"1234");
     QTest::keyClicks(&nombre,"Abraham");
     QTest::keyClicks(&appaterno,"rodriguez");
     QTest::keyClicks(&apmaterno,"alarcon");
     QTest::keyClicks(&fechaN,"1996-02-02");
     QTest::keyClicks(&edad,"23");
     QTest::keyClicks(&email,"a@gmail.com");
     QTest::keyClicks(&telefono,"1234567");
     QTest::keyClicks(&fotop,"C:/imagnes tips/agua.jpg");
     QTest::keyClicks(&idpregunta,"1");
     QTest::keyClicks(&respuesta,"pop");
     QTest::keyClicks(&idPuesto,"3");
     QTest::keyClicks(&estado,"1");
     QSqlQuery insertar,insertarp;
   insertar.prepare("insert into usuario(matricula,clave,nombre,appaterno,apmaterno,fechaN,edad,email,telefono,fotop,idpregunta,respuesta)"
     "values('"+matricula.text()+"','"+clave.text()+"','"+nombre.text()+"','"+appaterno.text()+"','"+apmaterno.text()+"',"
   "'"+fechaN.text()+"','"+edad.text()+"','"+email.text()+"','"+telefono.text()+"',LOAD_FILE('"+fotop.text()+"'),"
       "'"+idpregunta.text()+"','"+respuesta.text()+"')");
      if(insertar.exec()){
    if(insertarp.prepare(("insert into staff(idstaff,idUser,idpuesto,estado) value ('NULL','"+matricula.text()+"',"+idPuesto.text()+","+estado.text()+")"))){
        qDebug()<<"Staff registrado";
        qDebug()<<"Staff registrado en tabla staff";
           }
           else{
               qDebug()<<"Staff no registrado";
               qDebug()<<"Staff no registrado en tabla staff";
           }
      }
        mDatabase.close();
}*/
/*void prueba1::registrarDoctor(){
    QSqlDatabase mDatabase;
   mDatabase=QSqlDatabase::addDatabase("QMYSQL");
    mDatabase.setHostName("localhost");
    mDatabase.setDatabaseName("lobohospital");
    mDatabase.setPort(3306);
    mDatabase.setUserName("root");
    mDatabase.setPassword("");
    mDatabase.open();
     QLineEdit matricula,clave,nombre,appaterno,apmaterno,fechaN,edad,email,telefono,fotop,idpregunta,respuesta,
             especiDoc,horario,idpuesto,cedula,universidad,idconsultorio,estado;
     QTest::keyClicks(&matricula,"20193389");
     QTest::keyClicks(&clave,"1234");
     QTest::keyClicks(&nombre,"Jorge");
     QTest::keyClicks(&appaterno,"Benitez");
     QTest::keyClicks(&apmaterno,"alarcon");
     QTest::keyClicks(&fechaN,"1988-02-02");
     QTest::keyClicks(&edad,"30");
     QTest::keyClicks(&email,"jb24@gmail.com");
     QTest::keyClicks(&telefono,"1234567");
     QTest::keyClicks(&fotop,"C:/imagnes tips/omega.jpg");
     QTest::keyClicks(&idpregunta,"1");
     QTest::keyClicks(&respuesta,"pooh");
     QTest::keyClicks(&especiDoc,"1");
     QTest::keyClicks(&horario,"Matutino");
     QTest::keyClicks(&idpuesto,"1");
     QTest::keyClicks(&cedula,"3234923489");
     QTest::keyClicks(&universidad,"BUAP");
     QTest::keyClicks(&idconsultorio,"1");
     QTest::keyClicks(&estado,"1");
     QSqlQuery insertar,insertarp;
   insertar.prepare("insert into usuario(matricula,clave,nombre,appaterno,apmaterno,fechaN,edad,email,telefono,fotop,idpregunta,respuesta)"
     "values('"+matricula.text()+"','"+clave.text()+"','"+nombre.text()+"','"+appaterno.text()+"','"+apmaterno.text()+"',"
   "'"+fechaN.text()+"','"+edad.text()+"','"+email.text()+"','"+telefono.text()+"',LOAD_FILE('"+fotop.text()+"'),"
       "'"+idpregunta.text()+"','"+respuesta.text()+"')");
      if(insertar.exec()){
    if(insertarp.prepare(("insert into doctor(iddoctor,idUser,idEspecialidad,horario,idpuesto,cedulapro,"
                          "universidad,idconsultorio,estado)"
        "values(NULL,'"+matricula.text()+"','"+especiDoc.text()+"','"+horario.text()+"','"+idpuesto.text()+"',"
        "'"+cedula.text()+"','"+universidad.text()+"','"+idconsultorio.text()+"','"+estado.text()+"')"))){
        qDebug()<<"Doctor registrado";
        qDebug()<<"Doctor registrado en tabla doctor";
           }
           else{
               qDebug()<<"Doctor no registrado";
               qDebug()<<"Doctor no registrado en tabla doctor";
           }
      }
        mDatabase.close();
}*/


/*
void prueba1::loginTest()
{
    QLineEdit matricula,clave;
    QTest::keyClicks(&matricula,"20192384");
    QTest::keyClicks(&clave,"1234");
    QCOMPARE(matricula.text(),QString("20192384"));
    QCOMPARE(clave.text(),QString("1234"));
    qDebug () << "Usuario correcto";
}*/

void prueba1::loginPaciente(){
    QSqlDatabase mDatabase;
    mDatabase=QSqlDatabase::addDatabase("QMYSQL");
    mDatabase.setHostName("localhost");
    mDatabase.setDatabaseName("lobohospital");
    mDatabase.setPort(3306);
    mDatabase.setUserName("root");
    mDatabase.setPassword("");
    mDatabase.open();
    QLineEdit matricula,clave;
    QTest::keyClicks(&matricula,"3333333232");
    QTest::keyClicks(&clave,"1234");
    QString us=matricula.text();
    QString pass=clave.text();
    QSqlQuery select;
    QString nombre;
    select.prepare("SELECT * FROM usuario where matricula='"+ us +"'and clave='"+pass+"'");
    QCOMPARE(select.exec(),true);
    if(select.exec()){
        select.next();
         nombre=select.value(0).toString();
        if(nombre==us){
            qDebug()<<"paciente logueado";
        }else{
            qDebug()<<"Paciente no logueado";

        }
     }
    mDatabase.close();
}

/*
void prueba1::loginStaff(){
    QSqlDatabase mDatabase;
    mDatabase=QSqlDatabase::addDatabase("QMYSQL");
    mDatabase.setHostName("localhost");
    mDatabase.setDatabaseName("lobohospital");
    mDatabase.setPort(3306);
    mDatabase.setUserName("root");
    mDatabase.setPassword("");
    mDatabase.open();
    QLineEdit matricula,clave;
    QTest::keyClicks(&matricula,"2222222222");
    QTest::keyClicks(&clave,"1234");
    QString us=matricula.text();
    QString pass=clave.text();
    QSqlQuery select;
    QString nombre;
    select.prepare("SELECT * FROM usuario where matricula='"+ us +"'and clave='"+pass+"'");
    QCOMPARE(select.exec(),true);
    if(select.exec()){
        select.next();
         nombre=select.value(0).toString();
        if(nombre==us){
            qDebug()<<"Staff logueado";
        }else{
            qDebug()<<"Staff no logueado";

        }
     }
    mDatabase.close();
}*/
/*
void prueba1::loginDoctor(){
    QSqlDatabase mDatabase;
    mDatabase=QSqlDatabase::addDatabase("QMYSQL");
    mDatabase.setHostName("localhost");
    mDatabase.setDatabaseName("lobohospital");
    mDatabase.setPort(3306);
    mDatabase.setUserName("root");
    mDatabase.setPassword("");
    mDatabase.open();
    QLineEdit matricula,clave;
    QTest::keyClicks(&matricula,"232329");
    QTest::keyClicks(&clave,"1234");
    QString us=matricula.text();
    QString pass=clave.text();
    QSqlQuery select;
    QString nombre;
    select.prepare("SELECT * FROM usuario where matricula='"+ us +"'and clave='"+pass+"'");
    QCOMPARE(select.exec(),true);
    if(select.exec()){
        select.next();
         nombre=select.value(0).toString();
        if(nombre==us){
            qDebug()<<"Doctor logueado";
        }else{
            qDebug()<<"Doctor no logueado";

        }
     }
    mDatabase.close();
}*/


QTEST_MAIN(prueba1)
#include "testqstring.moc"










