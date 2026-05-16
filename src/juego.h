#ifndef JUEGO_H
#define JUEGO_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QList>

class Tejo;
class QMouseEvent;

class Juego : public QMainWindow {
    Q_OBJECT
public:
    Juego(QWidget *parent = nullptr);
    ~Juego();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void actualizarFisica();

private:
    QGraphicsScene *escena;
    QGraphicsView  *vista;

    QGraphicsPixmapItem *m_mocho;
    QList<Tejo*>         m_tejos;
    QTimer              *m_timerFisica;
    double               m_dtSegundos;
};

#endif // JUEGO_H
