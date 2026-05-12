#include "juego.h"
#include <QPainter>
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>

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

    // Dibujar un círculo amarillo en el centro de la escena
    QGraphicsEllipseItem *circulo = new QGraphicsEllipseItem(0, 0, 50, 50);
    circulo->setBrush(QBrush(QColor(255, 220, 50)));     // amarillo
    circulo->setPen(QPen(Qt::black, 2));                  // borde negro
    circulo->setPos(400 - 25, 300 - 25);                  // centro de 800x600
    escena->addItem(circulo);

}




Juego::~Juego() {}
