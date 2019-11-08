#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include "registrodoctor.h"
#include "tipdeldia.h"
#include <QMessageBox>
#include <QtSql/QSqlError>
#include <QtDebug>
#include <QFile>
#include <QFileDialog>
#include "registrostaff.h"
#include "registropaciente.h"
#include <QtSql/QSqlQueryModel>
#include "login.h"
#include "agregaractividaddoctor.h"
#include "agregarcitaspaciente.h"
#include "agregartipremedio.h"
#include "pagarcitaspaciente.h"
#include <QTimer>
#include "mostrarhistoclinico.h"
#include "agregarUrgencia.h"

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
    QString imgRoute,matric,idremedios,idtip,idtipeditar;
    QSqlQuery *datosPac, *datosDoc, *datosStaff, *datosUser;
    QString concolor;
    int UserTipo;
    bool verificarPasswordRegistro();
    QString calcularEdad(QString);
    bool verificarDatosRegistro();
    void ocultarMenuP();
    void mostrarMenuP();
    void cargarDatosUsuarios();
    void cargarHorarioDoc();
    void SolicitudCitas();
    void cargarCategoria();
    void CitasAceptadas();
    QList<QString> medicinas;
    QList<QString> porciones;
    QList<QString> frecMedicinas;
    int contMedicinas;
    QString idCita1;
    pagarCitasPaciente *pagar;
    QTimer *ocultar;
    int contador;


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

    void on_pushButton_SolicitudesUsuarios_clicked();

    void on_pushButton_AceptarSoli_clicked();

    void on_pushButton_regresarSolicitudes_clicked();

    void on_comboBox_area_currentTextChanged(const QString &arg1);

    void on_pushButton_guardar_clicked();

    void PonerInfo(QString);

    void infoConsultorio(QString);


    void on_pushButton_rechazarsoli_clicked();

    void on_radioButton_doctors_clicked();

    void on_radioButton_staffs_clicked();

    void on_pushButton_menu_Pincipal_Adm_clicked();

    void aceptarCita(QString);

    void rechazarCita(QString);
    void verCita(QString);


    void on_regresar_citasDoc_clicked();

    void on_butonNotifi_clicked();

    void on_radioCitaPersonal_clicked();

    void on_radioCitaExterna_clicked();

    void on_pushButton_clicked();

    void on_btnBuscarDoctor_clicked();

    void on_btnAgendarCita_clicked();

    void on_tv_listaDocCitas_clicked(const QModelIndex &index);

    void on_fechaCita_userDateChanged(const QDate &date);

    void on_tv_listaDocCitas_doubleClicked(const QModelIndex &index);

    void on_horaCita_activated(const QString &arg1);

    void on_btnEditarStaff_clicked();

    void on_btnCancelarEditarStaff_clicked();

    void on_btnGuardarEditarStaff_clicked();

    void on_btnMostrarContrasena_clicked();

    void on_btnMostrarContrasena_2_clicked();

    void on_pushButton_tip_2_clicked();

    void on_pushButton_agregar_tip_clicked();

    void on_pushButton_agregarImagen_clicked();

    void on_pushButton_menu_admin_clicked();

    void on_pushButton_agregar_remedio_clicked();

    void on_pushButton_menu_admin_2_clicked();

    void on_pushButton_agregar_remedio_2_clicked();

    void on_pushButton_Imgremedio_clicked();

    void on_pushButton_agergar_remedio_clicked();

    void on_pushButton_eliminar_remedio_clicked();

    void on_pushButton_editar_remedio_clicked();

    void on_comboBox_eliminarREmedios_currentTextChanged(const QString &arg1);

    void on_pushButton_eliminarRemedio_clicked();

    void on_comboBox_categoriaEdit_currentTextChanged(const QString &arg1);

    void on_pushButton_editarRemedio_clicked();

    void on_pushButton_agregartips_clicked();

    void on_pushButton_eliminarTips_clicked();

    void on_pushButton_eliminartip_clicked();

    void on_pushButton_editarTip_clicked();

    void mostrarTipsEditar();
    void mostrarTipsEliminar();
    void mostrarRemedioEliminar();
    void mostarrRemedioEditar();
    void Ponertips(QString);
    void PonerRemedios(QString);
    void ponerTipseliminar(QString);
    void on_pushButton_editarTips_clicked();

    void on_pb_rechazarCitas_clicked();
    void CancelarCita(QString);
    void on_btnEditarDoctor_clicked();

    void on_btnCancelarEditarDoctor_clicked();

    void on_btnGuardarEditarDoctor_clicked();

    void on_btnMostrarContrasena_3_clicked();

    void on_btnMostrarContrasena_4_clicked();

    void on_btnEditarPaciente_clicked();

    void on_btnCancelarEditarPaciente_clicked();

    void on_btnGuardarEditarPaciente_clicked();

    void on_btnMostrarContrasena_5_clicked();

    void on_btnMostrarContrasena_6_clicked();

    void PonerCitas(QString);
    void PagarCitas(QString);

    void mostrarCitas();

    void on_pushButton_Cancelar_Cita_clicked();

    void on_butonNotifi_4_clicked();
    void on_pb_llenarHistorial_clicked();

    void on_pb_buscarCita_clicked();

    void on_pb_realizarConsulta_clicked();

    void on_pb_receta_clicked();

    void on_pb_masMedicina_clicked();

    void quitarMedicina(int);
    void on_btnCitasCanceladas_clicked();

    void eliminarCita(QString);

    void cambiarVentana(QString);

    void on_btnCancelarAgenda_2_clicked();

    void on_pushButton_menu_admin_3_clicked();
    void docDisp(QString);

    void on_tv_listaDocCitas_2_clicked(const QModelIndex &index);

    void on_tv_listaDocCitas_2_doubleClicked(const QModelIndex &index);

    void on_btnAgendarCita_2_clicked();

    void actTablaCitas();

    void on_pushButton_historial_clicked();

    void on_pushButton_buscarHistMedico_clicked();

    void on_verHistoClinico_clicked();

    void historialClinico(QString);

    void mostrarHistorialClinico();

    void on_pushButton_2_clicked();

    void on_pb_addUrgencia_clicked();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void solicitUsuarios();
    QString mactri;
    int userTipo;
    int toggleVision;
    int toggleVision1;
   void mostrarZonas();
   void mostrasConsultorios();
   int verNoti;

};

#endif // MAINWINDOW_H
