#ifndef RUTAS_H
#define RUTAS_H

#include <QVector>
#include <QPointF>
#include "ciudades.h"
#include <QString>
#include <QMap>
#include <QSet>
#include <QGraphicsScene>
#include <QListWidget>
#include <QLabel>

extern QMap<QString, QPair<int, int>> pesos;
void inicializarPesos();

struct Ruta {
    QString nombre;
    NodoAVL* inicio;
    NodoAVL* fin;
    QVector<QPointF> puntos;
    double distancia;
    double tiempo;

};

class Rutas {
public:
    QMap<QString, QList<QString>> paradasFijas; // Estructura para paradas fijas
    void inicializarParadasFijas(); // Método para definir paradas fijas
    QList<QString> obtenerParadasFijas(const QString& origen, const QString& destino);
    Rutas(QGraphicsScene* scene);
    static QVector<Ruta> obtenerRutas();
    static QVector<QPointF> calcularRutaOptima(const QString& origen, const QString& destino, bool porTiempo);
    void dibujarRuta(const QString& origen, const QString& destino, QGraphicsScene* scene,QListWidget* Lw_Rutas, QColor colorLinea);
    void seleccionarRuta(QListWidget* Lw_Rutas, QLabel* Lb_KmRestante, QLabel* Lb_TiempoRestante, QLabel* Lb_HoraLlegada);

};

#endif // RUTAS_H
