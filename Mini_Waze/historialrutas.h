#ifndef HISTORIALRUTAS_H
#define HISTORIALRUTAS_H

#include "arbol_rn.h"
#include <QString>
#include <QList>

class HistorialRutas {
public:
    HistorialRutas(const QString& archivo = "historial_rutas.txt");
    void agregarRuta(const QString& ruta);
    QList<QString> obtenerHistorial() const;
    void limpiarHistorial();
    void guardarEnArchivo() const;
    void cargarDesdeArchivo();

private:
    QString archivoHistorial;
    ArbolRN arbolRutas;
};

#endif // HISTORIALRUTAS_H
