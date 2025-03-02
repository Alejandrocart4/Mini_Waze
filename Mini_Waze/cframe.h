#ifndef CFRAME_H
#define CFRAME_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "arbol_avl.h"

QT_BEGIN_NAMESPACE
namespace Ui { class cframe; }
QT_END_NAMESPACE

class cframe : public QMainWindow
{
    Q_OBJECT

public:
    explicit cframe(QWidget *parent = nullptr);
    ~cframe();

private:
    Ui::cframe *ui;
    QGraphicsScene *scene;

    void addCity(const QString &name, int x, int y);
    void  dibujarRutas();
    void agregarCiudades();
    void iniciarSimulacion();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void agregarCiudadesRecursivo(NodoAVL* nodo);

};

#endif // CFRAME_H
