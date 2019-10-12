#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_verRegistros_clicked()
{
    //Página de login
    ui->stackedWidget_registros->setCurrentIndex(1);
}

void MainWindow::on_label_logo_linkActivated(const QString &link)
{
    //Página principal
    ui->stackedWidget_principal->setCurrentIndex(0);
}
