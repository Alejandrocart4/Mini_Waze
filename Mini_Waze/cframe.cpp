/*#include "cframe.h"
#include "ui_cframe.h"
#include "rutas.h"
#include "ciudades.h"
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>


cframe::cframe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cframe)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    QPixmap mapa(":/Imagenes/Mapa de Honduras.jpg");
    QGraphicsPixmapItem *fondo = scene->addPixmap(mapa);
    //fondo->setPos(0,0);
    fondo->setZValue(-1);

    ui->graphicsView->setSceneRect(mapa.rect());

    dibujarRutas();
    Ciudades::inicializarCiudades();
    agregarCiudades();
    iniciarSimulacion();
}

cframe::~cframe()
{
    delete ui;
}


void cframe::agregarCiudades() {
    agregarCiudadesRecursivo(Ciudades::obtenerRaiz());
}

void cframe::agregarCiudadesRecursivo(NodoAVL* nodo) {
    if (!nodo) return;

    QGraphicsEllipseItem *city = scene->addEllipse(nodo->posicion.x(), nodo->posicion.y(),
                                                   10, 10, QPen(Qt::black), QBrush(Qt::blue));
    if (nodo->nombre == "Macuelizo") {
        QGraphicsTextItem *label = scene->addText(nodo->nombre, QFont("Arial", 10, QFont::Bold));
        label->setDefaultTextColor(Qt::black);
        label->setPos(nodo->posicion.x() - 50, nodo->posicion.y() - 20);
        label->setZValue(20);
    }

    city->setToolTip(nodo->nombre);
    city->setZValue(10);

    // Recursivamente agregar las ciudades hijas
    agregarCiudadesRecursivo(nodo->izquierda);
    agregarCiudadesRecursivo(nodo->derecha);
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

const int OFFSET_X = -9;//6
const int OFFSET_Y = -11;//3

void cframe::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = ui->graphicsView->mapToScene(event->pos());
    QPointF correctedPos(scenePos.x() + OFFSET_X, scenePos.y() + OFFSET_Y);

    //qDebug() << "Coordenada original: (" << scenePos.x() << "," << scenePos.y() << ")";
    qDebug() << "Coordenada corregida: (" << correctedPos.x() << "," << correctedPos.y() << ")";
}

void cframe::iniciarSimulacion() {
    QVector<Ruta> rutas = Rutas::obtenerRutas();
    if (rutas.isEmpty()) return;

    QVector<QPointF> ruta = rutas[0].puntos;

    QPixmap carroPixmap(":/Imagenes/carro1.png");
    QGraphicsPixmapItem *carro = scene->addPixmap(carroPixmap);
    carro->setScale(0.20);
    carro->setZValue(20);

    carro->setPos(ruta[0]);

    QSequentialAnimationGroup *animacionRuta = new QSequentialAnimationGroup(this);

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
}*/

#include "cframe.h"
#include "ui_cframe.h"
#include "rutas.h"
#include "ciudades.h"
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QMessageBox>

cframe::cframe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::cframe)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    QPixmap mapa(":/Imagenes/Mapa de Honduras.jpg");
    QGraphicsPixmapItem *fondo = scene->addPixmap(mapa);
    fondo->setZValue(-1);

    ui->graphicsView->setSceneRect(mapa.rect());

    Ciudades::inicializarCiudades();
    //agregarCiudades();

    ui->comboBox_Origen->addItem("");
    ui->comboBox_Destino->addItem("");

    NodoAVL* raiz = Ciudades::obtenerRaiz();
    QList<NodoAVL*> ciudadesPendientes = {raiz};
    while (!ciudadesPendientes.isEmpty()) {
        NodoAVL* nodo = ciudadesPendientes.takeFirst();
        ui->comboBox_Origen->addItem(nodo->nombre);
        ui->comboBox_Destino->addItem(nodo->nombre);

        if (nodo->izquierda) ciudadesPendientes.append(nodo->izquierda);
        if (nodo->derecha) ciudadesPendientes.append(nodo->derecha);
    }

    ui->comboBox_transporte->clear();
    ui->comboBox_transporte->addItems({"Privado", "Público"});

    connect(ui->comboBox_Origen, &QComboBox::currentTextChanged, this, &cframe::actualizarPuntoOrigen);
    connect(ui->comboBox_Destino, &QComboBox::currentTextChanged, this, &cframe::actualizarPuntoDestino);
}

cframe::~cframe() {
    delete ui;
}

void cframe::agregarCiudadesRecursivo(NodoAVL* nodo, const QString& origen, const QString& destino) {
    if (!nodo) return;

    if (nodo->nombre == origen || nodo->nombre == destino) {
        QGraphicsEllipseItem *city = scene->addEllipse(nodo->posicion.x(), nodo->posicion.y(),
                                                       10, 10, QPen(Qt::black), QBrush(Qt::blue));
        city->setToolTip(nodo->nombre);
        city->setZValue(10);
    }

    agregarCiudadesRecursivo(nodo->izquierda, origen, destino);
    agregarCiudadesRecursivo(nodo->derecha, origen, destino);
}

void cframe::actualizarPuntoOrigen(const QString& ciudad) {
    NodoAVL* nodo = Ciudades::buscarCiudad(ciudad);
    if (!nodo) return;

    scene->addEllipse(nodo->posicion.x(), nodo->posicion.y(), 10, 10,
                      QPen(Qt::black), QBrush(Qt::blue))->setZValue(20);
}

void cframe::actualizarPuntoDestino(const QString& ciudad) {
    NodoAVL* nodo = Ciudades::buscarCiudad(ciudad);
    if (!nodo) return;

    scene->addEllipse(nodo->posicion.x(), nodo->posicion.y(),
                      10, 10, QPen(Qt::black), QBrush(Qt::blue))->setZValue(20);
}

void cframe::on_btnRutas_clicked() {
    QString origen = ui->comboBox_Origen->currentText();
    QString destino = ui->comboBox_Destino->currentText();

    qDebug() << "Botón 'Rutas' presionado.";
    qDebug() << "Origen seleccionado: " << origen;
    qDebug() << "Destino seleccionado: " << destino;

    if (origen.isEmpty() || destino.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debes seleccionar ambas ciudades.");
        return;
    }

    if (origen == destino) {
        QMessageBox::warning(this, "Error", "La ciudad de origen y destino no pueden ser la misma.");
        return;
    }

    NodoAVL* nodoOrigen = Ciudades::buscarCiudad(origen);
    NodoAVL* nodoDestino = Ciudades::buscarCiudad(destino);

    if (!nodoOrigen) {
        qDebug() << "ERROR: No se encontró la ciudad de origen: " << origen;
        QMessageBox::critical(this, "Error", "No se encontró la ciudad de origen.");
        return;
    }

    if (!nodoDestino) {
        qDebug() << "ERROR: No se encontró la ciudad de destino: " << destino;
        QMessageBox::critical(this, "Error", "No se encontró la ciudad de destino.");
        return;
    }

    // Limpiar la escena y mostrar el mapa
    scene->clear();
    QPixmap mapa(":/Imagenes/Mapa de Honduras.jpg");
    scene->addPixmap(mapa)->setZValue(-1);

    // Agregar solo las ciudades seleccionadas
    agregarCiudadesRecursivo(Ciudades::obtenerRaiz(), origen, destino);

    // Obtener todas las rutas disponibles
    QVector<Ruta> rutasDisponibles = Rutas::obtenerRutas();
    qDebug() << "Total de rutas obtenidas: " << rutasDisponibles.size();

    ui->List_rutas->clear();
    indiceRutas.clear(); // Limpiar el vector antes de agregar nuevas rutas

    bool rutaEncontrada = false;

    // Agregar rutas válidas a la lista y dibujarlas en el mapa
    for (int i = 0; i < rutasDisponibles.size(); ++i) {
        if (rutasDisponibles[i].inicio && rutasDisponibles[i].fin) {
            if (rutasDisponibles[i].inicio->nombre == origen && rutasDisponibles[i].fin->nombre == destino) {
                qDebug() << "Agregando ruta: " << rutasDisponibles[i].nombre << " con índice real: " << i;
                ui->List_rutas->addItem(rutasDisponibles[i].nombre);
                indiceRutas.append(i);

                // Dibujar la ruta en el mapa
                Rutas rutas(scene);
                rutas.dibujarRuta(origen, destino, scene, ui->List_rutas, Qt::red);
                rutaEncontrada = true;
            }
        } else {
            qDebug() << "ERROR: Ruta inválida con índice: " << i;
        }
    }

    if (!rutaEncontrada) {
        QMessageBox::information(this, "Información", "No se encontraron rutas entre las ciudades seleccionadas.");
    }
}

void cframe::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = ui->graphicsView->mapToScene(event->pos());
    qDebug() << "Coordenada clickeada:" << scenePos;
}

void cframe::on_btn_seleccionRuta_clicked()
{
    int index = ui->List_rutas->currentRow();
    QString tipoTransporte = ui->comboBox_transporte->currentText();

    if (index == -1) {
        QMessageBox::warning(this, "Error", "Debes seleccionar una ruta de la lista.");
        return;
    }

    QVector<Ruta> rutasDisponibles = Rutas::obtenerRutas();

    if (index < 0 || index >= rutasDisponibles.size()) {
        QMessageBox::critical(this, "Error", "La ruta seleccionada no es válida.");
        return;
    }

    Ruta rutaSeleccionada = rutasDisponibles[index];

    if (!rutaSeleccionada.inicio || !rutaSeleccionada.fin) {
        QMessageBox::critical(this, "Error", "Error al obtener los datos de la ruta.");
        return;
    }

    if (carro) {
        if (carro->scene() == scene) {
            scene->removeItem(carro);
        }
        delete carro;
        carro = nullptr;
    }

    ui->Lb_KmRestante->setText(QString("Distancia: %1 km").arg(rutaSeleccionada.distancia));
    ui->Lb_TiempoRestante->setText(QString("Tiempo: %1 min").arg(rutaSeleccionada.tiempo));
    ui->Lb_HoraLlegada->setText("Hora de llegada estimada: 18:00");


    qDebug() << "Ruta seleccionada: " << rutaSeleccionada.nombre;
}

void cframe::iniciarSimulacion(const QVector<QPointF>& ruta, int index, const QString& tipoTransporte) {
    if (ruta.isEmpty()) {
        QMessageBox::warning(this, "Error", "No hay ruta disponible para la simulación.");
        return;
    }

    // Eliminar el vehículo anterior si existe
    if (carro) {
        if (carro->scene() == scene) {
            scene->removeItem(carro);
        }
        delete carro;
        carro = nullptr;
    }

    // Cargar la imagen del vehículo según el tipo de transporte
    QString imagenCarro = (tipoTransporte == "Público") ? ":/Imagenes/bus.png" : ":/Imagenes/carro1.png";

    QPixmap carroPixmap(imagenCarro);
    if (carroPixmap.isNull()) {
        QMessageBox::critical(this, "Error", "No se pudo cargar la imagen del vehículo.");
        return;
    }

    // Agregar el vehículo a la escena y colocarlo en la posición inicial de la ruta seleccionada
    carro = scene->addPixmap(carroPixmap);
    carro->setScale((tipoTransporte == "Público") ? 0.15 : 0.20);
    carro->setZValue(20);

    // Posicionar el vehículo en el punto de inicio de la ruta seleccionada
    carro->setPos(ruta.first());
    carro->setPos(ruta[0]);

    // Crear la animación de la ruta
    QSequentialAnimationGroup *animacionRuta = new QSequentialAnimationGroup(this);

    for (int i = index + 1; i < ruta.size(); ++i) {
        QVariantAnimation *animacion = new QVariantAnimation(this);
        animacion->setDuration(500);
        animacion->setStartValue(ruta[i - 1]);
        animacion->setEndValue(ruta[i]);


        connect(animacion, &QVariantAnimation::valueChanged, this, [this](const QVariant &value) {
            if (!this->carro) return;

            QPointF pos = value.toPointF();
            QSize carroSize = this->carro->pixmap().size();

            // Ajuste de posición para centrar el vehículo
            pos.setX(pos.x() - (carroSize.width() * 0.090));
            pos.setY(pos.y() - (carroSize.height() * 0.080));

            this->carro->setPos(pos);
        });

        animacionRuta->addAnimation(animacion);
    }

    animacionRuta->start();
}

void cframe::on_btnIniciarSimulacion_clicked()
{
    int indexLista = ui->List_rutas->currentRow();
    if (indexLista == -1) {
        QMessageBox::warning(this, "Error", "Debes seleccionar una ruta de la lista.");
        return;
    }

    if (indexLista < 0 || indexLista >= indiceRutas.size()) {
        QMessageBox::critical(this, "Error", "Índice de ruta seleccionado inválido.");
        return;
    }

    int indexRuta = indiceRutas[indexLista];  // Obtener el índice real en el QVector<Ruta>

    QVector<Ruta> rutasDisponibles = Rutas::obtenerRutas();
    if (indexRuta < 0 || indexRuta >= rutasDisponibles.size()) {
        QMessageBox::critical(this, "Error", "La ruta seleccionada no es válida.");
        return;
    }

    Ruta rutaSeleccionada = rutasDisponibles[indexRuta];

    if (!rutaSeleccionada.inicio || !rutaSeleccionada.fin || rutaSeleccionada.puntos.isEmpty()) {
        QMessageBox::critical(this, "Error", "No se encontró una ruta válida para animar.");
        return;
    }
    // Actualizar labels con la información de la ruta seleccionada
   /*ui->Lb_KmRestante->setText(QString::number(rutaSeleccionada.distancia) + " km");
    ui->Lb_TiempoRestante->setText(QString::number(rutaSeleccionada.tiempo) + " min");

    QTime horaActual = QTime::currentTime();
    QTime horaLlegada = horaActual.addSecs(rutaSeleccionada.tiempo * 60);
    ui->Lb_HoraLlegada->setText(horaLlegada.toString("hh:mm"));*/

    // Llamar a iniciarSimulacion con la ruta seleccionada
     iniciarSimulacion(rutaSeleccionada.puntos, 0, ui->comboBox_transporte->currentText());
}


