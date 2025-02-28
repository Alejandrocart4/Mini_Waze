#include "cframe.h"
#include "ui_cframe.h"
#include "rutas.h"
#include "ciudades.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>


cframe::cframe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cframe)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    // Crear la escena para el mapa
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    // Cargar el mapa de Honduras
    QPixmap mapa(":/Imagenes/Mapa de Honduras.jpg");
    QGraphicsPixmapItem *fondo = scene->addPixmap(mapa);
    fondo->setPos(0,0);
    fondo->setZValue(-1);

    ui->graphicsView->setSceneRect(mapa.rect());

    dibujarRutas();
    agregarCiudades();
    iniciarSimulacion();
}

cframe::~cframe()
{
    delete ui;
}


void cframe::agregarCiudades() {
    QVector<Ciudad> ciudades = Ciudades::obtenerCiudades();

    for (const auto& ciudad : ciudades) {
        QGraphicsEllipseItem *city = scene->addEllipse(ciudad.posicion.x(), ciudad.posicion.y(),
                                                       10, 10, QPen(Qt::black), QBrush(Qt::blue));
        city->setToolTip(ciudad.nombre);
        city->setZValue(10);

    }
}

void cframe::dibujarRutas() {
    QVector<Ruta> rutas = Rutas::obtenerRutas();

    for (const Ruta &ruta : rutas) {
        if (ruta.puntos.isEmpty()) continue;

        QPainterPath path;
        path.moveTo(ruta.puntos.first());
        for (const QPointF &punto : ruta.puntos) {
            path.lineTo(punto);
        }

        scene->addPath(path, QPen(Qt::red, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        qDebug() << "Ruta dibujada:" << ruta.nombre;
    }
}


const int OFFSET_X = -15;//-14
const int OFFSET_Y = -14;

void cframe::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = ui->graphicsView->mapToScene(event->pos());
    QPointF correctedPos(scenePos.x() + OFFSET_X, scenePos.y() + OFFSET_Y);

    //qDebug() << "Coordenada original: (" << scenePos.x() << "," << scenePos.y() << ")";
    qDebug() << "Coordenada corregida: (" << correctedPos.x() << "," << correctedPos.y() << ")";
}

void cframe::iniciarSimulacion() {
    QVector<Ruta> rutas = Rutas::obtenerRutas();
    if (rutas.isEmpty()) return;

    // Tomar la primera ruta (puedes modificar para elegir otra)
    QVector<QPointF> ruta = rutas[0].puntos;

    // Cargar la imagen del carro
    QPixmap carroPixmap(":/Imagenes/carro1.png");
    QGraphicsPixmapItem *carro = scene->addPixmap(carroPixmap);
    carro->setScale(0.20);
    carro->setZValue(20);

    // Posicionar el carro en el primer punto de la ruta
    carro->setPos(ruta[0]);

    // Crear un grupo de animaciones para seguir la ruta
    QSequentialAnimationGroup *animacionRuta = new QSequentialAnimationGroup(this);

    // Recorrer los puntos de la ruta y agregar animaciones
    for (int i = 1; i < ruta.size(); ++i) {
        QVariantAnimation *animacion = new QVariantAnimation(this);
        animacion->setDuration(500);
        animacion->setStartValue(ruta[i - 1]);
        animacion->setEndValue(ruta[i]);

        connect(animacion, &QVariantAnimation::valueChanged, this, [carro](const QVariant &value) {
            QPointF pos = value.toPointF();
            QSize carroSize = carro->pixmap().size();

            pos.setX(pos.x() - (carroSize.width() * 0.090));
            pos.setY(pos.y() - (carroSize.height() * 0.080));

            carro->setPos(pos);
        });

        animacionRuta->addAnimation(animacion);
    }

    animacionRuta->start();
}
