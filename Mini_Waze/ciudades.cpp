#include "ciudades.h"

QVector<Ciudad> Ciudades::obtenerCiudades() {
    QVector<Ciudad> ciudades;

    ciudades.append({"Tegucigalpa", {439, 499}});//1
    ciudades.append({"San Pedro Sula", {287, 211}});//2
    ciudades.append({"La Ceiba", {522, 168}});//3
    ciudades.append({"Choluteca", {441, 650}});//4
    ciudades.append({"Comayagua", {360 , 431}});//5
    ciudades.append({"Santa Rosa", {139 , 361}});//6
    ciudades.append({"Gracias",{179 , 401}});//7
    ciudades.append({"Nacaome",{385 , 609}});//8
    ciudades.append({"La Esperanza",{254 , 457}});//9
    ciudades.append({"Marcala",{284 , 488}});//10
    ciudades.append({"Catacamas",{733 , 333}});//11
    ciudades.append({"Tocoa",{666 , 204}});//12
    ciudades.append({"Olanchito",{566 , 223}});//13
    ciudades.append({"El Progreso",{327 , 242}});//14
    ciudades.append({"Nueva Ocotepeque",{61 , 426}});//15
    ciudades.append({"Danlí",{562 , 510}});//16
    ciudades.append({"Macuelizo",{182 , 265}});//17
    ciudades.append({"Tela",{402 , 166}});//18

    return ciudades;
}
