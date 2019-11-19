#ifndef REMEDIOS_H
#define REMEDIOS_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
class remedios;
}

class remedios : public QDialog
{
    Q_OBJECT

public:
    explicit remedios(QString  ,QString ,QString ,QByteArray,QWidget *parent = nullptr);
    ~remedios();
    void llenarInformacion();

private:
    Ui::remedios *ui;
    QString titulo,ingredientes,procedimiento;
    QByteArray foto;
};

#endif // REMEDIOS_H
