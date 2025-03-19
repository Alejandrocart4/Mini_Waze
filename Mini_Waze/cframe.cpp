#include "cframe.h"
#include "ui_cframe.h"
#include "rutas.h"
#include "ciudades.h"
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QMessageBox>
#include <QTime>
#include <QTimer>
#include <QMap>
#include <QString>
#include "manejadospesos.h"
#include "arbol_rn.h"
#include "historialrutas.h"

HistorialRutas historial;

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

    rutas = new Rutas(scene);

    ui->comboBox_transporte->clear();
    ui->comboBox_transporte->addItems({"Privado", "Público"});
    connect(ui->comboBox_Origen, &QComboBox::currentTextChanged, this, &cframe::actualizarPuntoOrigen);
    connect(ui->comboBox_Destino, &QComboBox::currentTextChanged, this, &cframe::actualizarPuntoDestino);

    //connect(ui->btn_limpiarHistorial, &QPushButton::clicked, this, &cframe::on_btn_limpiarHistorial_clicked);

    mostrarHistorial();
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
    ui->List_rutas->clear();
    indiceRutas.clear();
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
    if (rutasDisponibles.isEmpty()) {
        QMessageBox::warning(this, "Error", "No se encontraron rutas disponibles.");
        return;
    }

    // Agregar rutas válidas a la lista y dibujarlas en el mapa
    bool rutaEncontrada = false;
    QSet<QString> rutasMostradas; // Almacenar rutas ya agregadas para evitar duplicados

    for (int i = 0; i < rutasDisponibles.size(); ++i) {
        Ruta ruta = rutasDisponibles[i];

        if (!ruta.inicio || !ruta.fin) continue;

        // Verificar si la ruta coincide exactamente con el origen y destino seleccionados
        if (ruta.inicio->nombre == origen && ruta.fin->nombre == destino) {
            QString nombreRuta = origen + " - " + destino;
            ui->List_rutas->addItem(nombreRuta);  // Agregar la ruta al listado
            indiceRutas.append(i);

            // Dibujar la ruta en el mapa con los puntos correctos
            QPen pen(Qt::red);
            pen.setWidth(3);
            QPainterPath path;
            path.moveTo(ruta.puntos[0]);
            for (const QPointF& punto : ruta.puntos) {
                path.lineTo(punto);
            }
            scene->addPath(path, pen);

            rutaEncontrada = true;
            break;  // No seguir buscando después de encontrar la ruta
        }
    }

    ui->Lw_ParadasFijas->clear();
    QList<QString> paradasFijas = rutas->obtenerParadasFijas(origen, destino);
    if (!paradasFijas.isEmpty()) {
        ui->Lw_ParadasFijas->addItems(paradasFijas);
    } else {
        ui->Lw_ParadasFijas->addItem("No hay paradas fijas.");
    }

    if (!rutaEncontrada) {
        QMessageBox::information(this, "Información", "No se encontraron rutas entre las ciudades seleccionadas.");
    }
}

void cframe::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = ui->graphicsView->mapToScene(event->pos());
    qDebug() << "Coordenada clickeada:" << scenePos;
}

void cframe::mostrarHistorial() {
    ui->listHistorial->clear();
    QList<QString> rutas = historial.obtenerHistorial();
    for (const QString& ruta : rutas) {
        ui->listHistorial->addItem(ruta);
    }
}

void cframe::on_btn_seleccionRuta_clicked()
{
    QString origen = ui->comboBox_Origen->currentText();
    QString destino = ui->comboBox_Destino->currentText();

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

    ManejadorPesos manejador;

    // Obtener los pesos usando el manejador
    QPair<int, int> datos = manejador.obtenerPesos(origen, destino);
    int distancia = datos.first;
    int duracion = datos.second;

     QString ruta = origen + " - " + destino;

    // Verificar si se encontró una ruta válida
    if (distancia > 0 && duracion > 0) {
        int horas = duracion / 60;
        int minutos = duracion % 60;

        // Formato de distancia
        QString distanciaTexto = QString("Distancia: %1 km").arg(distancia);

        // Formato de duración
        QString tiempoTexto;
        if (horas > 0) {
            tiempoTexto = QString("Duración: %1 h %2 min").arg(horas).arg(minutos);
        } else {
            tiempoTexto = QString("Duración: %1 min").arg(minutos);
        }

        // Calcular la hora de llegada
        QTime horaActual = QTime::currentTime();
        QTime horaLlegada = horaActual.addSecs(duracion * 60);
        QString horaLlegadaTexto = QString("Hora de llegada estimada: %1").arg(horaLlegada.toString("hh:mm"));

        // Actualizar los QLabel con la información formateada
        ui->Lb_KmRestante->setText(distanciaTexto);
        ui->Lb_TiempoRestante->setText(tiempoTexto);
        ui->Lb_HoraLlegada->setText(horaLlegadaTexto);
    } else {
        ui->Lb_KmRestante->setText("Ruta no encontrada");
        ui->Lb_TiempoRestante->setText("");
        ui->Lb_HoraLlegada->setText("");
    }

    historial.agregarRuta(ruta);
    mostrarHistorial();

    qDebug() << "Ruta seleccionada: " << rutaSeleccionada.nombre;
}

void cframe::limpiarUI() {
    if (carro) {
        if (carro->scene() == scene) {
            scene->removeItem(carro);
        }
        delete carro;
        carro = nullptr;
    }

    scene->clear();
    QPixmap mapa(":/Imagenes/Mapa de Honduras.jpg");
    scene->addPixmap(mapa)->setZValue(-1);

    // Resetear los elementos de la UI
    ui->comboBox_Origen->setCurrentIndex(0);
    ui->comboBox_Destino->setCurrentIndex(0);
    ui->comboBox_transporte->setCurrentIndex(0);
    ui->List_rutas->clear();
    ui->Lw_ParadasFijas->clear();
    ui->Lb_KmRestante->clear();
    ui->Lb_TiempoRestante->clear();
    ui->Lb_HoraLlegada->clear();
}

void cframe::iniciarSimulacion(const QVector<QPointF>& ruta, int index, const QString& tipoTransporte) {
    /*if (ruta.isEmpty()) {
        QMessageBox::warning(this, "Error", "No hay ruta disponible para la simulación.");
        return;
    }

    QString origen = ui->comboBox_Origen->currentText();
    QString destino = ui->comboBox_Destino->currentText();

    // Obtener las paradas fijas de la ruta seleccionada
    QList<QString> paradasFijas = rutas->obtenerParadasFijas(origen, destino);

    QList<QAbstractAnimation*> animaciones = findChildren<QAbstractAnimation*>();
    for (QAbstractAnimation* anim : animaciones) {
        qDebug() << "Eliminando animación previa";
        anim->stop();
        anim->deleteLater();
    }

    // Eliminar el vehículo anterior de manera segura
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

    carro = scene->addPixmap(carroPixmap);
    carro->setScale((tipoTransporte == "Público") ? 0.15 : 0.20);
    carro->setZValue(20);
    carro->setPos(ruta.first());

    QPointF inicioPos = ruta.first();
    carro->setPos(inicioPos);

    // Crear la animación de la ruta
    QSequentialAnimationGroup *animacionRuta = new QSequentialAnimationGroup(this);
    qDebug() << "Creando nuevo grupo de animación:" << animacionRuta;

    connect(animacionRuta, &QSequentialAnimationGroup::finished, this, [this, animacionRuta]() {
        QMessageBox::information(this, "Simulación", "Ha llegado a su destino.");
        limpiarUI();
        animacionRuta->deleteLater();
    });

    for (int i = index + 1; i < ruta.size(); ++i) {
        QVariantAnimation *animacion = new QVariantAnimation(this);
        animacion->setDuration(500);
        animacion->setStartValue(ruta[i - 1]);
        animacion->setEndValue(ruta[i]);

         animacion->setEasingCurve(QEasingCurve::Linear);

       // qDebug() << "Creando animación para paso:" << i << "Animación:" << animacion;

        connect(animacion, &QVariantAnimation::valueChanged, this, [this](const QVariant &value) {
             if (carro) {

            QPointF pos = value.toPointF();
            QSize carroSize = this->carro->pixmap().size();

            // Ajuste de posición para centrar el vehículo
            pos.setX(pos.x() - (carroSize.width() * 0.090));
            pos.setY(pos.y() - (carroSize.height() * 0.080));

            this->carro->setPos(pos);
             }
        });

        animacionRuta->addAnimation(animacion);
        connect(animacion, &QVariantAnimation::finished, animacion, &QVariantAnimation::deleteLater);
        animacionRuta->addAnimation(animacion);

        // 🚏 Verificar si el punto actual es una parada fija
        for (const auto& parada : paradasFijas) {
            NodoAVL* nodoParada = Ciudades::buscarCiudad(parada);
            if (nodoParada && ruta[i] == nodoParada->posicion) {
                // Detener la animación por 1 segundo
                QTimer::singleShot(1000, this, [animacionRuta]() {
                    animacionRuta->start();
                });
                return; // Evita que la animación continúe sin pausa
            }
        }
    }

     animacionRuta->start(QAbstractAnimation::DeleteWhenStopped);*/
    if (ruta.isEmpty()) {
        QMessageBox::warning(this, "Error", "No hay ruta disponible para la simulación.");
        return;
    }

    QString origen = ui->comboBox_Origen->currentText();
    QString destino = ui->comboBox_Destino->currentText();

    // 🔥 Eliminar cualquier animación previa (si existe)
    QList<QAbstractAnimation*> animaciones = findChildren<QAbstractAnimation*>();
    for (QAbstractAnimation* anim : animaciones) {
        qDebug() << "Eliminando animación previa";
        anim->stop();
        anim->deleteLater();
    }

    // 🔥 Eliminar el vehículo anterior de manera segura
    if (carro) {
        if (carro->scene() == scene) {
            scene->removeItem(carro);
        }
        delete carro;
        carro = nullptr;
    }

    // 🔥 Cargar la imagen del vehículo según el tipo de transporte
    QString imagenCarro = (tipoTransporte == "Público") ? ":/Imagenes/bus.png" : ":/Imagenes/carro1.png";
    QPixmap carroPixmap(imagenCarro);
    if (carroPixmap.isNull()) {
        QMessageBox::critical(this, "Error", "No se pudo cargar la imagen del vehículo.");
        return;
    }

    // 🔥 Crear el objeto del carro desde cero
    carro = scene->addPixmap(carroPixmap);
    carro->setScale((tipoTransporte == "Público") ? 0.15 : 0.20);
    carro->setZValue(20);

    // 🔥 Reiniciar la posición del carro al origen de la ruta
    QPointF inicioPos = ruta.first();
    QSize carroSize = carro->pixmap().size();
    inicioPos.setX(inicioPos.x() - (carroSize.width() * 0.090));
    inicioPos.setY(inicioPos.y() - (carroSize.height() * 0.080));
    carro->setPos(inicioPos);

    // 🔥 Crear un grupo de animación nuevo siempre
    QSequentialAnimationGroup *animacionRuta = new QSequentialAnimationGroup(this);
    qDebug() << "Creando nuevo grupo de animación:" << animacionRuta;

    // 🔥 Conectar la finalización de la animación
    connect(animacionRuta, &QSequentialAnimationGroup::finished, this, [this, animacionRuta]() {
        QMessageBox::information(this, "Simulación", "Ha llegado a su destino.");
        limpiarUI();
        animacionRuta->clear();    // Limpiar el grupo de animación
        animacionRuta->deleteLater(); // Eliminar el grupo de animación
    });

    // 🔥 Crear las animaciones de la ruta
    for (int i = index + 1; i < ruta.size(); ++i) {
        QVariantAnimation *animacion = new QVariantAnimation(this);

        // 🔥 Duración ajustada para garantizar que no sea instantánea
        animacion->setDuration(500);
        animacion->setStartValue(ruta[i - 1]);
        animacion->setEndValue(ruta[i]);

        // 🔥 Asegurar interpolación lineal para movimiento suave
        animacion->setEasingCurve(QEasingCurve::Linear);

        // 🔥 Conectar el movimiento del carro
        connect(animacion, &QVariantAnimation::valueChanged, this, [this](const QVariant &value) {
            if (carro) {
                QPointF pos = value.toPointF();
                QSize carroSize = carro->pixmap().size();
                pos.setX(pos.x() - (carroSize.width() * 0.090));
                pos.setY(pos.y() - (carroSize.height() * 0.080));
                carro->setPos(pos);
            }
        });

        animacionRuta->addAnimation(animacion);
    }

    // 🔥 Configuración de grupo de animación nuevo y seguro
    animacionRuta->setLoopCount(1);
    animacionRuta->start(QAbstractAnimation::DeleteWhenStopped);

    qDebug() << "Iniciando animación:" << animacionRuta;
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

    qDebug() << "Ruta seleccionada (antes de iniciar):";
    for (const auto& punto : rutaSeleccionada.puntos) {
        qDebug() << "Punto:" << punto;
    }

    // Llamar a iniciarSimulacion con la ruta seleccionada
     iniciarSimulacion(rutaSeleccionada.puntos, 0, ui->comboBox_transporte->currentText());
}

void cframe::on_btn_limpiarHistorial_clicked()
{
    historial.limpiarHistorial();
    mostrarHistorial();
}

