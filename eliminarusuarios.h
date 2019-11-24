#ifndef ELIMINARUSUARIOS_H
#define ELIMINARUSUARIOS_H

#include <QDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QDate>
#include <QMessageBox>

namespace Ui {
class eliminarUsuarios;
}

class eliminarUsuarios : public QDialog
{
    Q_OBJECT

public:
    explicit eliminarUsuarios(QWidget *parent = nullptr);
    ~eliminarUsuarios();
    void eliminarDoc(QString);
        QString idUser, idStaff, idPaciente, idDoctor,especialidad;
    void eliminarStaff(QString);

    void eliminarPaciente(QString);

    void limpiar();

private slots:
    void on_radioButton_doc_clicked();

    void on_radioButton_staff_clicked();

    void on_radioButton_pac_clicked();

    void ModificarUsuario(QString, QString);

    void on_pushButton_buscar_clicked();

private:
    Ui::eliminarUsuarios *ui;

};

#endif // ELIMINARUSUARIOS_H
