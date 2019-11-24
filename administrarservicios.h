#ifndef ADMINISTRARSERVICIOS_H
#define ADMINISTRARSERVICIOS_H

#include <QDialog>

namespace Ui {
class AdministrarServicios;
}

class AdministrarServicios : public QDialog
{
    Q_OBJECT

public:
    explicit AdministrarServicios(QWidget *parent = nullptr);
    ~AdministrarServicios();

private:
    Ui::AdministrarServicios *ui;
};

#endif // ADMINISTRARSERVICIOS_H
