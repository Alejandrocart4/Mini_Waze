#include "arbol_rn.h"

// Constructor del árbol rojo-negro
ArbolRN::ArbolRN() {
    // Crear un nodo nulo (TNULL) para representar hojas vacías
    TNULL = new NodoRN("", NEGRO);
    TNULL->izquierdo = nullptr;
    TNULL->derecho = nullptr;
    raiz = TNULL;
}

// Destructor del árbol rojo-negro
ArbolRN::~ArbolRN() {
    limpiar();
    delete TNULL;
}

// Inicializar nodo nulo
void ArbolRN::inicializarNodoNulo(NodoRN* nodo, NodoRN* padre) {
    nodo->valor = "";
    nodo->color = NEGRO;
    nodo->padre = padre;
    nodo->izquierdo = nullptr;
    nodo->derecho = nullptr;
}

// Realizar una rotación a la izquierda
void ArbolRN::rotacionIzquierda(NodoRN* x) {
    NodoRN* y = x->derecho;
    x->derecho = y->izquierdo;
    if (y->izquierdo != TNULL) {
        y->izquierdo->padre = x;
    }
    y->padre = x->padre;
    if (x->padre == nullptr) {
        raiz = y;
    } else if (x == x->padre->izquierdo) {
        x->padre->izquierdo = y;
    } else {
        x->padre->derecho = y;
    }
    y->izquierdo = x;
    x->padre = y;
}

// Realizar una rotación a la derecha
void ArbolRN::rotacionDerecha(NodoRN* x) {
    NodoRN* y = x->izquierdo;
    x->izquierdo = y->derecho;
    if (y->derecho != TNULL) {
        y->derecho->padre = x;
    }
    y->padre = x->padre;
    if (x->padre == nullptr) {
        raiz = y;
    } else if (x == x->padre->derecho) {
        x->padre->derecho = y;
    } else {
        x->padre->izquierdo = y;
    }
    y->derecho = x;
    x->padre = y;
}

// Insertar un nodo en el árbol rojo-negro
void ArbolRN::insertar(const QString& valor) {
    NodoRN* nuevo = new NodoRN(valor, ROJO);
    nuevo->izquierdo = TNULL;
    nuevo->derecho = TNULL;
    nuevo->padre = nullptr;

    NodoRN* padre = nullptr;
    NodoRN* actual = raiz;

    // Búsqueda del lugar de inserción
    while (actual != TNULL) {
        padre = actual;
        if (valor < actual->valor) {
            actual = actual->izquierdo;
        } else if (valor > actual->valor) {
            actual = actual->derecho;
        } else {
            qDebug() << "Ruta ya existente:" << valor;
            delete nuevo;
            return;
        }
    }

    nuevo->padre = padre;
    if (padre == nullptr) {
        raiz = nuevo;
    } else if (valor < padre->valor) {
        padre->izquierdo = nuevo;
    } else {
        padre->derecho = nuevo;
    }

    // Si el nodo es raíz, marcarlo como negro
    if (nuevo->padre == nullptr) {
        nuevo->color = NEGRO;
        return;
    }

    if (nuevo->padre->padre == nullptr) {
        return;
    }

    // Ajustar el balanceo después de la inserción
    balancearInsercion(nuevo);
}

// Balancear el árbol después de la inserción
void ArbolRN::balancearInsercion(NodoRN* nodo) {
    NodoRN* tio;
    while (nodo->padre->color == ROJO) {
        if (nodo->padre == nodo->padre->padre->izquierdo) {
            tio = nodo->padre->padre->derecho;
            if (tio->color == ROJO) {
                nodo->padre->color = NEGRO;
                tio->color = NEGRO;
                nodo->padre->padre->color = ROJO;
                nodo = nodo->padre->padre;
            } else {
                if (nodo == nodo->padre->derecho) {
                    nodo = nodo->padre;
                    rotacionIzquierda(nodo);
                }
                nodo->padre->color = NEGRO;
                nodo->padre->padre->color = ROJO;
                rotacionDerecha(nodo->padre->padre);
            }
        } else {
            tio = nodo->padre->padre->izquierdo;
            if (tio->color == ROJO) {
                nodo->padre->color = NEGRO;
                tio->color = NEGRO;
                nodo->padre->padre->color = ROJO;
                nodo = nodo->padre->padre;
            } else {
                if (nodo == nodo->padre->izquierdo) {
                    nodo = nodo->padre;
                    rotacionDerecha(nodo);
                }
                nodo->padre->color = NEGRO;
                nodo->padre->padre->color = ROJO;
                rotacionIzquierda(nodo->padre->padre);
            }
        }
        if (nodo == raiz) break;
    }
    raiz->color = NEGRO;
}

// Limpiar el árbol completo
void ArbolRN::limpiar() {
    limpiarRecursivo(raiz);
    raiz = TNULL;
}

// Eliminar recursivamente todos los nodos
void ArbolRN::limpiarRecursivo(NodoRN* nodo) {
    if (nodo != TNULL) {
        limpiarRecursivo(nodo->izquierdo);
        limpiarRecursivo(nodo->derecho);
        delete nodo;
    }
}

// Imprimir el árbol para depuración
void ArbolRN::imprimirArbol(NodoRN* nodo, QString indent, bool ultimo) const {
    if (nodo != TNULL) {
        qDebug() << indent << (ultimo ? "└── " : "├── ") << nodo->valor
                 << "(" << (nodo->color == ROJO ? "Rojo" : "Negro") << ")";
        indent += ultimo ? "    " : "│   ";
        imprimirArbol(nodo->izquierdo, indent, false);
        imprimirArbol(nodo->derecho, indent, true);
    }
}

void ArbolRN::enOrdenRec(NodoRN* nodo, QList<QString>& lista) const {
    if (nodo != TNULL) {
        enOrdenRec(nodo->izquierdo, lista);
        lista.append(nodo->valor);
        enOrdenRec(nodo->derecho, lista);
    }
}

// Recorrido en orden (público)
QList<QString> ArbolRN::enOrden() const {
    QList<QString> lista;
    enOrdenRec(raiz, lista);
    return lista;
}

NodoRN* ArbolRN::obtenerRaiz() const {
    return raiz;
}
