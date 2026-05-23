#ifndef JUEGO_H
#define JUEGO_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QList>

class Tejo;
class Mecha;
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
    void detectarColisiones();
    void actualizarHUD();

    QGraphicsScene *escena;
    QGraphicsView  *vista;

    QGraphicsPixmapItem *m_mocho;
    QList<Tejo*>         m_tejos;
    QList<Mecha*>        m_mechas;
    QTimer              *m_timerFisica;
    double               m_dtSegundos;
    int                  m_puntaje;
    int                  m_tejosRestantes;
    QGraphicsTextItem   *m_textoPuntaje;
    QGraphicsTextItem   *m_textoTejos;
};

#endif // JUEGO_H
