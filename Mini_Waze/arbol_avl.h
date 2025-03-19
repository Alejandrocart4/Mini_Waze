#ifndef ARBOL_AVL_H
#define ARBOL_AVL_H

#include <QString>
#include <QPointF>

struct NodoAVL {
    QString nombre;
    QPointF posicion;
    QString alias;
    NodoAVL *izquierda;
    NodoAVL *derecha;
    int altura;

    NodoAVL(const QString& nombre, const QPointF& posicion)
        : nombre(nombre), posicion(posicion), izquierda(nullptr), derecha(nullptr), altura(1) {}
};

class ArbolAVL {
public:
    ArbolAVL();
    ~ArbolAVL();
    void insertar(const QString& nombre, const QPointF& posicion);
    NodoAVL* buscar(const QString& nombre);
    void imprimirInOrden();
    NodoAVL* obtenerRaiz();

private:
    NodoAVL* raiz;
    int altura(NodoAVL* nodo);
    int obtenerBalance(NodoAVL* nodo);
    NodoAVL* rotacionDerecha(NodoAVL* y);
    NodoAVL* rotacionIzquierda(NodoAVL* x);
    NodoAVL* insertarNodo(NodoAVL* nodo, const QString& nombre, const QPointF& posicion);
    void eliminarArbol(NodoAVL* nodo);
    void imprimirInOrden(NodoAVL* nodo);
};

#endif // ARBOL_AVL_H
