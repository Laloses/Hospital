#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include "registrodoctor.h"
#include "tipdeldia.h"
 #include <QMessageBox>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
    QString id_usuario,id_staff,id_doctor,id_paciente;
    QByteArray foto;
    QString imgRoute,matric;
    QSqlQuery *datosPac, *datosDoc, *datosStaff, *datosUser;
    int UserTipo;
    bool verificarPasswordRegistro();
    QString calcularEdad(QString);
    bool verificarDatosRegistro();
    void ocultarMenuP();
    void mostrarMenuP();
    void cargarDatosUsuarios();


private slots:
    void on_pushButton_verRegistros_clicked();

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

    void on_pushButton_SolicitudesUsuarios_clicked();

    void on_pushButton_AceptarSoli_clicked();

    void on_pushButton_regresarSolicitudes_clicked();

    void on_comboBox_area_currentTextChanged(const QString &arg1);

    void on_pushButton_guardar_clicked();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void PonerInfo(QString);
    void infoConsultorio(QString);
    void solicitUsuarios();
    QString mactri;
    int userTipo;
   void mostrarZonas();
   void mostrasConsultorios();

};

#endif // MAINWINDOW_H
