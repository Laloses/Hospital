#ifndef DIALOGINFODOC_H
#define DIALOGINFODOC_H

#include <QDialog>

namespace Ui {
class DialogInfoDoc;
}

class DialogInfoDoc : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInfoDoc(QString,QString,QString,QString,QString,QString,QString,QByteArray, QWidget *parent = nullptr);
    ~DialogInfoDoc();
    void llenarDatos();


private:
    Ui::DialogInfoDoc *ui;
    QString nombre,especialidad,consul,celu,email,horario,cedu;
    QByteArray fotodoc;


};

#endif // DIALOGINFODOC_H
