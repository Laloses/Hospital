#ifndef ACEPTARSOLICITUD_H
#define ACEPTARSOLICITUD_H

#include <QDialog>

namespace Ui {
class aceptarSolicitud;
}

class aceptarSolicitud : public QDialog
{
    Q_OBJECT

public:
    explicit aceptarSolicitud(QWidget *parent = nullptr);
    ~aceptarSolicitud();

private:
    Ui::aceptarSolicitud *ui;
};

#endif // ACEPTARSOLICITUD_H
