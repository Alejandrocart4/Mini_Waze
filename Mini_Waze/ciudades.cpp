#include "ciudades.h"

ArbolAVL Ciudades::arbolCiudades;
void Ciudades::inicializarCiudades() {

    arbolCiudades.insertar("Tegucigalpa", {439, 499});//1*
    arbolCiudades.insertar("San Pedro Sula", {287, 211});//2*
    arbolCiudades.insertar("La Ceiba", {522, 168});//3*
    arbolCiudades.insertar("Choluteca", {441, 650});//4*
    arbolCiudades.insertar("Comayagua", {360 , 431});//5
    arbolCiudades.insertar("Santa Rosa de Copán", {139 , 361});//6*
    arbolCiudades.insertar("Gracias",{179 , 401});//7*
    arbolCiudades.insertar("Nacaome",{385 , 609});//8*
    arbolCiudades.insertar("La Esperanza",{254 , 457});//9*
    arbolCiudades.insertar("Marcala",{284 , 488});//10*
    arbolCiudades.insertar("Catacamas",{733 , 333});//11*
    arbolCiudades.insertar("Tocoa",{666 , 204});//12*
    arbolCiudades.insertar("Olanchito",{564 , 223});//13*
    arbolCiudades.insertar("El Progreso",{327 , 242});//14*
    arbolCiudades.insertar("Nueva Ocotepeque",{61 , 426});//15*
    arbolCiudades.insertar("Danlí",{562 , 510});//16*
    arbolCiudades.insertar("Macuelizo",{182 , 265});//17*
    arbolCiudades.insertar("Tela",{402 , 166});//18*
    arbolCiudades.insertar("Guaimaca",{516 , 411});//19*
    arbolCiudades.insertar("Talanga",{468 , 434});//20*
    arbolCiudades.insertar("La Paz",{355 , 454 });//21*
    arbolCiudades.insertar("Yoro",{459 , 294});//22*
    arbolCiudades.insertar("Juticalpa",{696 , 358});//23*
    arbolCiudades.insertar("Siguatepeque",{345 , 410 });//24*
    arbolCiudades.insertar("La Entrada",{143 , 309});//25*
                                        //147 , 316
}

NodoAVL* Ciudades::buscarCiudad(const QString& nombre) {
    return arbolCiudades.buscar(nombre);
}

NodoAVL* Ciudades::obtenerRaiz() {
    return arbolCiudades.obtenerRaiz();
}
