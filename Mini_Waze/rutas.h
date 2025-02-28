#ifndef RUTAS_H
#define RUTAS_H

#include <QVector>
#include <QPointF>

struct Ruta {
    QString nombre;
    QVector<QPointF> puntos;
};

class Rutas {
public:
    static QVector<Ruta> obtenerRutas();
};

#endif // RUTAS_H
