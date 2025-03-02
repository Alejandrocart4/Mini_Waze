#ifndef RUTAS_H
#define RUTAS_H

#include <QVector>
#include <QPointF>
#include "ciudades.h"

struct Ruta {
    QString nombre;
    NodoAVL* inicio;
    NodoAVL* fin;
    QVector<QPointF> puntos;
};

class Rutas {
public:
    static QVector<Ruta> obtenerRutas();
};

#endif // RUTAS_H
