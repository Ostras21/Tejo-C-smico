#include "juego.h"
#include <QPainter>

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
}

Juego::~Juego() {}
