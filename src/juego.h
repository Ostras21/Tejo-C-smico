#ifndef JUEGO_H
#define JUEGO_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QList>
#include <QRandomGenerator>

class Tejo;
class Mecha;
class Roca;
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
    void actualizarTiempo();
    void cambiarViento();

private:
    void detectarColisiones();
    void actualizarHUD();

    QGraphicsScene *escena;
    QGraphicsView  *vista;

    QGraphicsPixmapItem *m_mocho;
    QList<Tejo*>         m_tejos;
    QList<Mecha*>        m_mechas;
    QList<Roca*>         m_rocas;
    QTimer              *m_timerFisica;
    double               m_dtSegundos;
    int                  m_puntaje;
    int                  m_tejosRestantes;
    int                  m_tiempoRestante;
    QTimer              *m_timerNivel;
    QTimer              *m_timerViento;
    double               m_vientoSolar;
    QGraphicsTextItem   *m_textoPuntaje;
    QGraphicsTextItem   *m_textoTejos;
    QGraphicsTextItem   *m_textoTiempo;
    QGraphicsTextItem   *m_textoViento;
};

#endif // JUEGO_H
