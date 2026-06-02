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
class Enemigo;
class ComandanteAlien;
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
    void terminarJuego(bool victoria, const QString &mensaje);
    void verificarFinJuego();
    void cargarNivel2();
    void limpiarEscena();

    QGraphicsScene *escena;
    QGraphicsView  *vista;

    QGraphicsPixmapItem *m_mocho;
    QList<Tejo*>         m_tejos;
    QList<Mecha*>        m_mechas;
    QList<Roca*>         m_rocas;
    QList<Enemigo*>      m_enemigos;
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
    bool                 m_juegoTerminado;
    QGraphicsTextItem   *m_textoResultado;
    QGraphicsTextItem   *m_textoSubresultado;
    int                  m_nivelActual;
    ComandanteAlien     *m_comandante;
};

#endif // JUEGO_H
