#ifndef DIALOGDOCTOR_H
#define DIALOGDOCTOR_H

#include <QDialog>
#include <QSql>
#include <QString>

namespace Ui {
class DialogDoctor;
}

class DialogDoctor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDoctor(QString,QString,QString,QString,QWidget *parent = nullptr);
    ~DialogDoctor();
    void llenarInfoDoc();

private slots:
    void on_masinfo_clicked();

private:
    Ui::DialogDoctor *ui;
    QString nombre,espc,consult,tel;
};

#endif // DIALOGDOCTOR_H
