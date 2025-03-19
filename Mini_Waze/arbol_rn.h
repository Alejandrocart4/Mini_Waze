#ifndef ARBOL_RN_H
#define ARBOL_RN_H

#include <QString>
#include <QList>
#include <QDebug>

enum Color { ROJO, NEGRO };

struct NodoRN {
    QString valor;
    Color color;
    NodoRN* izquierdo;
    NodoRN* derecho;
    NodoRN* padre;

    NodoRN(const QString& valor, Color color)
        : valor(valor), color(color), izquierdo(nullptr), derecho(nullptr), padre(nullptr) {}
};

class ArbolRN {
public:
    ArbolRN();
    ~ArbolRN();
    void insertar(const QString& valor);
    bool buscar(const QString& valor);
    QList<QString> enOrden() const;
    void limpiar();
    void imprimirArbol(NodoRN* nodo, QString indent = "", bool ultimo = true) const;

    NodoRN* obtenerRaiz() const;

private:
    NodoRN* raiz;
    NodoRN* TNULL;

    void inicializarNodoNulo(NodoRN* nodo, NodoRN* padre);
    void rotacionIzquierda(NodoRN* x);
    void rotacionDerecha(NodoRN* y);
    void balancearInsercion(NodoRN* nodo);
    void limpiarRecursivo(NodoRN* nodo);
    void enOrdenRec(NodoRN* nodo, QList<QString>& lista) const;
};

#endif // ARBOL_RN_H
