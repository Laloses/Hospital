#ifndef AGREGARUSUARIO_H
#define AGREGARUSUARIO_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtSql/QSqlError>
#include <QtDebug>
#include <QFile>
#include <QFileDialog>
#include "registrostaff.h"
#include "registropaciente.h"
#include "registrodoctor.h"
namespace Ui {
class AgregarUsuario;
}

class AgregarUsuario : public QMainWindow
{
    Q_OBJECT

public:
    explicit AgregarUsuario(QWidget *parent = nullptr);
    ~AgregarUsuario();
    bool verificarDatosRegistro();
    bool verificarPasswordRegistro();
    QString calcularEdad(QString);
    void limpiar();
    QByteArray foto;
    QString imgRoute;

private slots:
    void on_pushButton_registrarse_clicked();

    void on_pushButton_imgPerfil_clicked();

    void on_pushButton_respuesta_clicked();

    void on_pushButton_2_clicked();

    void on_radioButton_doc_toggled(bool checked);

    void on_radioButton_paciente_toggled(bool checked);

    void on_radioButton_staff_toggled(bool checked);

private:
    Ui::AgregarUsuario *ui;

};

#endif // AGREGARUSUARIO_H
