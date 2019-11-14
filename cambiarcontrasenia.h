#ifndef CAMBIARCONTRASENIA_H
#define CAMBIARCONTRASENIA_H

#include <QMainWindow>

namespace Ui {
class CambiarContrasenia;
}

class CambiarContrasenia : public QMainWindow
{
    Q_OBJECT

public:
    explicit CambiarContrasenia(QWidget *parent = nullptr);
    ~CambiarContrasenia();

private:
    Ui::CambiarContrasenia *ui;
};

#endif // CAMBIARCONTRASENIA_H
