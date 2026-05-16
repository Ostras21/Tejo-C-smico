#include "juego.h"
#include "tejo.h"

#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QtMath>

Juego::Juego(QWidget *parent)
    : QMainWindow(parent),
      escena(nullptr),
      vista(nullptr),
      m_mocho(nullptr),
      m_timerFisica(nullptr),
      m_dtSegundos(1.0 / 60.0)
{
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
    m_mocho = new QGraphicsPixmapItem(pixmapMocho);
    m_mocho->setPos(375, 400);
    escena->addItem(m_mocho);

    // Game loop: timer de física a ~60 FPS
    m_timerFisica = new QTimer(this);
    connect(m_timerFisica, &QTimer::timeout, this, &Juego::actualizarFisica);
    m_timerFisica->start(16);
}

Juego::~Juego() {}

void Juego::mousePressEvent(QMouseEvent *event) {
    // Posición del click en coordenadas de la escena
    const QPointF puntoEscena = vista->mapToScene(event->pos());

    const double mochoX = m_mocho->x();
    const double mochoY = m_mocho->y();

    // Ángulo desde el Mocho hacia el click (Y invertida para que "arriba" sea positivo)
    const double dx = puntoEscena.x() - mochoX;
    const double dy = mochoY - puntoEscena.y();
    const double angulo = qAtan2(dy, dx);

    // Crear el tejo un poco arriba del Mocho y lanzarlo
    Tejo *tejo = new Tejo();
    tejo->setPos(mochoX + 30, mochoY - 20);
    escena->addItem(tejo);
    m_tejos.append(tejo);
    tejo->lanzar(angulo, 700.0);

    QMainWindow::mousePressEvent(event);
}

void Juego::actualizarFisica() {
    // Actualizar todos los tejos y recoger los que se salieron de la escena
    for (int i = m_tejos.size() - 1; i >= 0; --i) {
        Tejo *tejo = m_tejos[i];
        tejo->actualizar(m_dtSegundos);

        const double tx = tejo->x();
        const double ty = tejo->y();
        if (tx < 0 || tx > 800 || ty < 0 || ty > 600) {
            escena->removeItem(tejo);
            m_tejos.removeAt(i);
            delete tejo;
        }
    }
}
