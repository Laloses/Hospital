#ifndef EDITARUSUARIO_H
#define EDITARUSUARIO_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QtDebug>
#include <QTimer>
#include <QStringList>
#include <QCompleter>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QtWidgets>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QFileSystemModel>

namespace Ui {
class EditarUsuario;
}

class EditarUsuario : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditarUsuario(QString, QString, QWidget *parent = nullptr);
    ~EditarUsuario();

private slots:
    void on_btnCancelar_clicked();

    void on_btnSiguiente_clicked();

    void on_btnAtrasDoc_clicked();

    void on_btnAtrasStaff_clicked();

    void on_btnFotoPerfil_clicked();

    void on_btnGuardarCambios_clicked();

    void on_btnGuardarCambiosDoctor_clicked();

    void on_btnGuardarCambiosStaff_clicked();

private:
    Ui::EditarUsuario *ui;

    QSqlDatabase database;
    QSqlQueryModel *model;

    QSqlQuery *datosUsr;

    QString mat;
    QString tipoUsur;

    QByteArray foto;
    QString imgRoute;

    QString nombreUsr;
    QString appatUst;
    QString apmatUsr;
    QString email;
    QString telefono;
    QPixmap img;
    QString pregunta;
    QString respuesta;
    QString religion;
    QString sexo;
    QString fotop;

};

#endif // EDITARUSUARIO_H
