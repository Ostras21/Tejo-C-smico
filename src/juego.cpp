#include "juego.h"
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QPixmap>

Juego::Juego(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Tejo Cósmico");
    resize(800, 600);

    // Crear la escena (el espacio donde viven los objetos del juego)
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 800, 600);
    escena->setBackgroundBrush(QColor(20, 20, 50));  // azul oscuro, como el espacio

    // Crear la vista (la cámara que muestra la escena)
    vista = new QGraphicsView(escena, this);
    vista->setRenderHint(QPainter::Antialiasing);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Hacer que la vista ocupe toda la ventana
    setCentralWidget(vista);

    // Cargar el sprite del Mocho desde el recurso Qt
    QPixmap pixmapMocho(":/sprites/nivel1/Mocho.png");
    if (pixmapMocho.isNull()) {
        qWarning("No se pudo cargar el sprite del Mocho desde :/sprites/nivel1/Mocho.png");
    }

    // Crear el item con la imagen y colocarlo en el centro inferior de la escena
    QGraphicsPixmapItem *mocho = new QGraphicsPixmapItem(pixmapMocho);
    mocho->setPos(375, 400);
    escena->addItem(mocho);

}




Juego::~Juego() {}
