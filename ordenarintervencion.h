#ifndef ORDENARINTERVENCION_H
#define ORDENARINTERVENCION_H

#include <QMainWindow>

namespace Ui {
class OrdenarIntervencion;
}

class OrdenarIntervencion : public QMainWindow
{
    Q_OBJECT

public:
    explicit OrdenarIntervencion(QWidget *parent = nullptr);
    ~OrdenarIntervencion();

private:
    Ui::OrdenarIntervencion *ui;
};

#endif // ORDENARINTERVENCION_H
