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
    QGraphicsPixmapItem *carro = nullptr;

    void agregarCiudades();
    void agregarCiudadesRecursivo(NodoAVL* nodo, const QString& origen, const QString& destino);
   void iniciarSimulacion(const QVector<QPointF>& ruta, int index, const QString& tipoTransporte);
    void dibujarRutas();
    QVector<int> indiceRutas;


protected:
    void mousePressEvent(QMouseEvent *event) override;
    //void agregarCiudadesRecursivo(NodoAVL* nodo);

private slots:
    void actualizarPuntoOrigen(const QString& ciudad);
    void actualizarPuntoDestino(const QString& ciudad);
    //void on_comboBox_transporte_currentTextChanged(const QString& transporte);
    void on_btnRutas_clicked();
    //void on_btnIniciarSimulacion_clicked();
    void on_btn_seleccionRuta_clicked();
    void on_btnIniciarSimulacion_clicked();
};

#endif // CFRAME_H
