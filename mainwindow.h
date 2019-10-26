#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include "registrodoctor.h"
#include "tipdeldia.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
    QSqlQueryModel *model;
    QString id_usuario,id_staff,id_doctor,id_paciente;
    QByteArray foto;
    QString imgRoute;
    QSqlQuery *datosPac, *datosDoc, *datosStaff, *datosUser;
    bool verificarPasswordRegistro();
    QString calcularEdad(QString);
    bool verificarDatosRegistro();
    void ocultarMenuP();
    void mostrarMenuP();
    void cargarDatosUsuarios();
    void cargarHorarioDoc();

private slots:

    void on_pushButton_logo_clicked();

    void on_pushButton_login_clicked();

    void on_pushButton_registro_clicked();

    void on_radioButton_doc_toggled(bool checked);

    void on_radioButton_staff_toggled(bool checked);

    void on_radioButton_paciente_toggled(bool checked);

    void on_pushButton_iniciarSesion_clicked();

    void on_pushButton_salir_clicked();

    void on_pushButton_registrarse_clicked();

    void on_pushButton_imgPerfil_clicked();

    void on_pushButton_respuesta_clicked();

    void on_pushButton_tip_clicked();

    void on_pushButton_miPerfil_clicked();

    void on_lineEdit_passwordLogin_returnPressed();

    void on_pushButton_citasDoc_clicked();

    void on_pushButton_horarioDoc_clicked();

    void on_pushButton_datosPaciente_clicked();

    void on_pb_agregarActividadDoc_clicked();

    void on_radioCitaPersonal_clicked();

    void on_radioCitaExterna_clicked();

    void on_pushButton_clicked();

    void on_btnBuscarDoctor_clicked();

    void on_btnAgendarCita_clicked();

    void on_tv_listaDocCitas_clicked(const QModelIndex &index);

    void on_fechaCita_userDateChanged(const QDate &date);

    void on_tv_listaDocCitas_doubleClicked(const QModelIndex &index);

    void on_horaCita_activated(const QString &arg1);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

};

#endif // MAINWINDOW_H
