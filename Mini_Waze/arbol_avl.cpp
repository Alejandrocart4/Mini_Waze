#include "arbol_avl.h"
#include <iostream>

ArbolAVL::ArbolAVL() : raiz(nullptr) {}

ArbolAVL::~ArbolAVL() {
    eliminarArbol(raiz);
}

void ArbolAVL::eliminarArbol(NodoAVL* nodo) {
    if (!nodo) return;
    eliminarArbol(nodo->izquierda);
    eliminarArbol(nodo->derecha);
    delete nodo;
}

int ArbolAVL::altura(NodoAVL* nodo) {
    return nodo ? nodo->altura : 0;
}

int ArbolAVL::obtenerBalance(NodoAVL* nodo) {
    return nodo ? altura(nodo->izquierda) - altura(nodo->derecha) : 0;
}

NodoAVL* ArbolAVL::rotacionDerecha(NodoAVL* y) {
    NodoAVL* x = y->izquierda;
    NodoAVL* T2 = x->derecha;

    x->derecha = y;
    y->izquierda = T2;

    y->altura = std::max(altura(y->izquierda), altura(y->derecha)) + 1;
    x->altura = std::max(altura(x->izquierda), altura(x->derecha)) + 1;

    return x;
}

NodoAVL* ArbolAVL::rotacionIzquierda(NodoAVL* x) {
    NodoAVL* y = x->derecha;
    NodoAVL* T2 = y->izquierda;

    y->izquierda = x;
    x->derecha = T2;

    x->altura = std::max(altura(x->izquierda), altura(x->derecha)) + 1;
    y->altura = std::max(altura(y->izquierda), altura(y->derecha)) + 1;

    return y;
}

NodoAVL* ArbolAVL::insertarNodo(NodoAVL* nodo, const QString& nombre, const QPointF& posicion) {
    if (!nodo) return new NodoAVL(nombre, posicion);

    if (nombre < nodo->nombre)
        nodo->izquierda = insertarNodo(nodo->izquierda, nombre, posicion);
    else if (nombre > nodo->nombre)
        nodo->derecha = insertarNodo(nodo->derecha, nombre, posicion);
    else
        return nodo;

    nodo->altura = 1 + std::max(altura(nodo->izquierda), altura(nodo->derecha));

    int balance = obtenerBalance(nodo);

    if (balance > 1 && nombre < nodo->izquierda->nombre)
        return rotacionDerecha(nodo);
    if (balance < -1 && nombre > nodo->derecha->nombre)
        return rotacionIzquierda(nodo);
    if (balance > 1 && nombre > nodo->izquierda->nombre) {
        nodo->izquierda = rotacionIzquierda(nodo->izquierda);
        return rotacionDerecha(nodo);
    }
    if (balance < -1 && nombre < nodo->derecha->nombre) {
        nodo->derecha = rotacionDerecha(nodo->derecha);
        return rotacionIzquierda(nodo);
    }

    return nodo;
}

void ArbolAVL::insertar(const QString& nombre, const QPointF& posicion) {
    raiz = insertarNodo(raiz, nombre, posicion);
}

NodoAVL* ArbolAVL::buscar(const QString& nombre) {
    NodoAVL* actual = raiz;
    while (actual) {
        if (nombre == actual->nombre) return actual;
        actual = (nombre < actual->nombre) ? actual->izquierda : actual->derecha;
    }
    return nullptr;
}

void ArbolAVL::imprimirInOrden() {
    imprimirInOrden(raiz);
}

void ArbolAVL::imprimirInOrden(NodoAVL* nodo) {
    if (!nodo) return;
    imprimirInOrden(nodo->izquierda);
    std::cout << nodo->nombre.toStdString() << " ";
    imprimirInOrden(nodo->derecha);
}

NodoAVL* ArbolAVL::obtenerRaiz() {
    return raiz;
}
