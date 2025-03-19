#include "historialrutas.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

HistorialRutas::HistorialRutas(const QString& archivo) : archivoHistorial(archivo) {
    cargarDesdeArchivo();
}

void HistorialRutas::agregarRuta(const QString& ruta) {
    arbolRutas.limpiar();  // Limpiar el árbol antes de insertar nuevas rutas
    arbolRutas.insertar(ruta);
    guardarEnArchivo();
    arbolRutas.imprimirArbol(arbolRutas.obtenerRaiz());  // Mostrar el árbol para depuración
}

QList<QString> HistorialRutas::obtenerHistorial() const {
    return arbolRutas.enOrden();  // Obtener el historial en orden
}

void HistorialRutas::limpiarHistorial() {
    arbolRutas.limpiar();  // Limpiar el árbol RN
    guardarEnArchivo();    // Limpiar el archivo
}

void HistorialRutas::guardarEnArchivo() const {
    QFile file(archivoHistorial);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        QList<QString> rutas = arbolRutas.enOrden();
        for (const QString& ruta : rutas) {
            out << ruta << "\n";
        }
        file.close();
    } else {
        qWarning() << "No se pudo abrir el archivo de historial para escritura.";
    }
}

void HistorialRutas::cargarDesdeArchivo() {
    QFile file(archivoHistorial);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString linea = in.readLine();
            arbolRutas.insertar(linea);
        }
        file.close();
    } else {
        qWarning() << "No se pudo abrir el archivo de historial para lectura.";
    }
}
