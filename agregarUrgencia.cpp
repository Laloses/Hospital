#include "agregarUrgencia.h"
#include "ui_agregarUrgencia.h"

agregarUrgencia::agregarUrgencia(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::agregarUrgencia)
{
    ui->setupUi(this);
    this->setWindowTitle("Urgencias");
    urgencias=new QSqlQuery;
    QSqlDatabase::database();
    //Buscamos todas las urgencias por fecha
    buscarUrgencias();
}

agregarUrgencia::~agregarUrgencia()
{
    delete ui;
}

void agregarUrgencia::buscarUrgencias(){
    QList<QSqlRecord> datos;
    QString consulta="SELECT idEmergencia,colorEmergencia,forma_llegada,fecha,hora "
                     "FROM urgencias ORDER BY fecha,hora";
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
        connect(pb,&QPushButton::clicked,[=](){emit verDetalles(id);});
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

void agregarUrgencia::verDetalles(QString id){
    ui->sw_urgencias->setCurrentIndex(1);
}
