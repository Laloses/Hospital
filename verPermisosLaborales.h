#ifndef VERPERMISOSLABORALES_H
#define VERPERMISOSLABORALES_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include <QPushButton>

namespace Ui {
class VerPermisosLaborales;
}

class VerPermisosLaborales : public QDialog
{
    Q_OBJECT

public:
    explicit VerPermisosLaborales(QWidget *parent = nullptr);
    ~VerPermisosLaborales();

private:
    Ui::VerPermisosLaborales *ui;
    void clearLayout(QLayout* lay);
    void rechazarPermisoLaboral(QString idPermiso, QString matricula);
    void cargarPermisos();
    void aceptarPermisoLaboral(QString idPermiso, QString matricula);
};

#endif // VERPERMISOSLABORALES_H
