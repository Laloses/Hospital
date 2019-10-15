#ifndef REGISTROSTAFF_H
#define REGISTROSTAFF_H
#include "qstring.h"
#include "QVariant"


class registroStaff
{
public:
    registroStaff(QString idStaff,
                  QString idUser,
                  QString idstaff,
                  QString idPuesto,
                  QString estado,
                  QString matricula,
                  QString clave,
                  QString nombre,
                  QString apePaterno,
                  QString apeMaterno,
                  QString fechaN,
                  QString edad,
                  QString email,
                  QString telefono,
                  QVariant fotop,
                  );
};

#endif // REGISTROSTAFF_H
