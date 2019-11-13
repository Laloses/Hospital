#ifndef ELIMINARUSUARIOS_H
#define ELIMINARUSUARIOS_H

#include <QDialog>

namespace Ui {
class eliminarUsuarios;
}

class eliminarUsuarios : public QDialog
{
    Q_OBJECT

public:
    explicit eliminarUsuarios(QWidget *parent = nullptr);
    ~eliminarUsuarios();

private:
    Ui::eliminarUsuarios *ui;
};

#endif // ELIMINARUSUARIOS_H
