#include "manejadospesos.h"

// Constructor que inicializa los pesos
ManejadorPesos::ManejadorPesos() {
    inicializarPesos();
}

// Inicialización de los pesos de las rutas
void ManejadorPesos::inicializarPesos() {
    // Rutas entre ciudades principales y secundarias
    pesos["San Pedro Sula-Tegucigalpa"] = {240, 300};  // 240 km, 5 horas
    pesos["Tegucigalpa-San Pedro Sula"] = {240, 300};

    pesos["San Pedro Sula-El Progreso"] = {30, 35};  // 30 km, 35 min
    pesos["El Progreso-San Pedro Sula"] = {30, 35};

    pesos["San Pedro Sula-La Ceiba"] = {192, 230};  // 192 km, 3 horas 50 min
    pesos["La Ceiba-San Pedro Sula"] = {192, 230};

    pesos["San Pedro Sula-Tocoa"] = {275, 330};  // 275 km, 5 horas 30 min
    pesos["Tocoa-San Pedro Sula"] = {275, 330};

    pesos["San Pedro Sula-Nueva Ocotepeque"] = {249, 300};  // 249 km, 5 horas
    pesos["Nueva Ocotepeque-San Pedro Sula"] = {249, 300};

    pesos["San Pedro Sula-Macuelizo"] = {140, 150};  // 140 km, 2 horas 30 min
    pesos["Macuelizo-San Pedro Sula"] = {140, 150};

    pesos["San Pedro Sula-Santa Rosa de Copan"] = {170, 150};  // 170 km, 2 horas 30 min
    pesos["Santa Rosa de Copan-San Pedro Sula"] = {170, 150};

    pesos["San Pedro Sula-Olanchito"] = {180, 200};  // 180 km, 3 horas 20 min
    pesos["Olanchito-San Pedro Sula"] = {180, 200};

    pesos["San Pedro Sula-Catacamas"] = {320, 400};  // 320 km, 6 horas 40 min
    pesos["Catacamas-San Pedro Sula"] = {320, 400};

    pesos["San Pedro Sula-Gracias"] = {210, 180};  // 210 km, 3 horas
    pesos["Gracias-San Pedro Sula"] = {210, 180};

    pesos["San Pedro Sula-Nacaome"] = {270, 360};  // 270 km, 6 horas
    pesos["Nacaome-San Pedro Sula"] = {270, 360};

    pesos["San Pedro Sula-Choluteca"] = {300, 420};  // 300 km, 7 horas
    pesos["Choluteca-San Pedro Sula"] = {300, 420};

    pesos["San Pedro Sula-Danlí"] = {240, 330};  // 240 km, 5 horas 30 min
    pesos["Danlí-San Pedro Sula"] = {240, 330};

    pesos["San Pedro Sula-La Paz"] = {170, 160};  // 170 km, 2 horas 40 min
    pesos["La Paz-San Pedro Sula"] = {170, 160};

    pesos["San Pedro Sula-Marcala"] = {210, 240};  // 210 km, 4 horas
    pesos["Marcala-San Pedro Sula"] = {210, 240};

    pesos["San Pedro Sula-La Esperanza"] = {270, 270};  // 270 km, 4 horas 30 minutos
    pesos["La Esperanza-San Pedro Sula"] = {270, 270};

    pesos["San Pedro Sula-Talanga"] = {150, 180};  // 150 km, 3 horas
    pesos["Talanga-San Pedro Sula"] = {150, 180};

    pesos["San Pedro Sula-Guaimaca"] = {180, 210};  // 180 km, 3 horas 30 min
    pesos["Guaimaca-San Pedro Sula"] = {180, 210};

    pesos["San Pedro Sula-Yoro"] = {164, 90};  // 164 km, 1 hora 30 min
    pesos["Yoro-San Pedro Sula"] = {164, 90};

    pesos["San Pedro Sula-Siguatepeque"] = {110, 100};  // 110 km, 1 hora 40 min
    pesos["Siguatepeque-San Pedro Sula"] = {110, 100};

    pesos["San Pedro Sula-Juticalpa"] = {220, 300};  // 220 km, 5 horas
    pesos["Juticalpa-San Pedro Sula"] = {220, 300};

    pesos["San Pedro Sula-Tela"] = {97, 105};  // 97 km, 1 hora 45 min
    pesos["Tela-San Pedro Sula"] = {97, 105};

    pesos["San Pedro Sula-Comayagua"] = {161, 140};  // 161 km, 2 horas 20 min
    pesos["Comayagua-San Pedro Sula"] = {161, 140};

    pesos["San Pedro Sula-La Entrada"] = {120, 120};  // 120 km, 2 horas
    pesos["La Entrada-San Pedro Sula"] = {120, 120};

    // Rutas entre ciudades secundarias
    pesos["Olanchito-Tocoa"] = {55, 65};  // 55 km, 1 hora 5 min
    pesos["Tocoa-Olanchito"] = {55, 65};

    pesos["Gracias-La Esperanza"] = {80, 100};  // 80 km, 1 hora 40 min
    pesos["La Esperanza-Gracias"] = {80, 100};

    pesos["Tocoa-Catacamas"] = {150, 180};  // 150 km, 3 horas
    pesos["Catacamas-Tocoa"] = {150, 180};

    pesos["Catacamas-Danlí"] = {105, 120};  // 105 km, 2 horas
    pesos["Danlí-Catacamas"] = {105, 120};

    pesos["Choluteca-Nacaome"] = {100, 110};  // 100 km, 1 hora 50 min
    pesos["Nacaome-Choluteca"] = {100, 110};

    pesos["La Esperanza-Tela"] = {190, 220};  // 190 km, 3 horas 40 min
    pesos["Tela-La Esperanza"] = {190, 220};

    pesos["Tegucigalpa-Ceiba"] = {385, 420};  // 385 km, 7 horas
    pesos["Ceiba-Tegucigalpa"] = {385, 420};

    pesos["Tela-El Progreso"] = {60, 70};  // 60 km, 1 hora 10 min
    pesos["El Progreso-Tela"] = {60, 70};

    pesos["El Progreso-Yoro"] = {90, 100};  // 90 km, 1 hora 40 min
    pesos["Yoro-El Progreso"] = {90, 100};
}

// Método para obtener los pesos dados el origen y destino
QPair<int, int> ManejadorPesos::obtenerPesos(const QString& origen, const QString& destino) const {
    QString clave = origen + "-" + destino;
    if (pesos.contains(clave)) {
        return pesos.value(clave);
    }
    return {0, 0};  // Retorna {0, 0} si no existe la ruta
}
