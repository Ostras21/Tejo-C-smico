#ifndef JUEGO_H
#define JUEGO_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>

class Juego : public QMainWindow {
    Q_OBJECT
public:
    Juego(QWidget *parent = nullptr);
    ~Juego();

private:
    QGraphicsScene *escena;
    QGraphicsView *vista;
};

#endif // JUEGO_H
