#ifndef ELIMINARUSUARIOS_H
#define ELIMINARUSUARIOS_H

#include <QDialog>
#include <QDebug>
#include <QSqlQuery>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
namespace Ui {
class eliminarUsuarios;
}

class eliminarUsuarios : public QDialog
{
    Q_OBJECT

public:
    explicit eliminarUsuarios(QWidget *parent = nullptr);
    ~eliminarUsuarios();

private slots:
    void on_radioButton_doc_clicked();

    void on_radioButton_staff_clicked();

    void eliminarDoc(QString);

    void eliminarStaff(QString);

    void eliminarPaciente(QString);

    void on_radioButton_pac_clicked();

    void ModificarUsuario(QString);

private:
    Ui::eliminarUsuarios *ui;
};

#endif // ELIMINARUSUARIOS_H
