#ifndef MANEJADORPESOS_H
#define MANEJADORPESOS_H

#include <QMap>
#include <QString>
#include <QPair>

class ManejadorPesos {
public:
    ManejadorPesos();  // Constructor para inicializar los pesos
    QPair<int, int> obtenerPesos(const QString& origen, const QString& destino) const;

private:
    QMap<QString, QPair<int, int>> pesos;  // Mapa para almacenar los pesos de las rutas
    void inicializarPesos();  // Método para inicializar los pesos
};

#endif // MANEJADORPESOS_H
