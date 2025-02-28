#ifndef CIUDADES_H
#define CIUDADES_H

#include <QVector>
#include <QPointF>
#include <QString>

struct Ciudad {
    QString nombre;
    QPointF posicion;
};

class Ciudades {
public:
    static QVector<Ciudad> obtenerCiudades();
};

#endif // CIUDADES_H
