#ifndef AGREGARURGENCIA_H
#define AGREGARURGENCIA_H

#include <QWidget>

namespace Ui {
class agregarUrgencia;
}

class agregarUrgencia : public QWidget
{
    Q_OBJECT

public:
    explicit agregarUrgencia(QWidget *parent = nullptr);
    ~agregarUrgencia();

private:
    Ui::agregarUrgencia *ui;
};

#endif // AGREGARURGENCIA_H
