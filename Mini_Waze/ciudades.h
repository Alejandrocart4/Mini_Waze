#ifndef CIUDADES_H
#define CIUDADES_H

#include <QString>
#include "arbol_avl.h"

class Ciudades {
private:
    static ArbolAVL arbolCiudades;
public:
    static void inicializarCiudades();
    static NodoAVL* buscarCiudad(const QString& nombre);
    static NodoAVL* obtenerRaiz();
};

#endif // CIUDADES_H
