#include "juego.h"
#include "tejo.h"
#include "mecha.h"
#include "roca.h"

#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QtMath>
#include <QVector>
#include <QPointF>
#include <QGraphicsTextItem>
#include <QFont>

Juego::Juego(QWidget *parent)
    : QMainWindow(parent),
      escena(nullptr),
      vista(nullptr),
      m_mocho(nullptr),
      m_timerFisica(nullptr),
      m_dtSegundos(1.0 / 60.0),
      m_puntaje(0),
      m_tejosRestantes(10),
      m_tiempoRestante(20),
      m_timerNivel(nullptr),
      m_timerViento(nullptr),
      m_vientoSolar(0.0),
      m_textoPuntaje(nullptr),
      m_textoTejos(nullptr),
      m_textoTiempo(nullptr),
      m_textoViento(nullptr),
      m_juegoTerminado(false),
      m_textoResultado(nullptr),
      m_textoSubresultado(nullptr)
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

    // Crear las mechas (objetivos) del lado derecho de la escena
    const QVector<QPointF> posicionesMechas = {
        QPointF(650, 280),
        QPointF(700, 350),
        QPointF(650, 420),
        QPointF(700, 490),
        QPointF(650, 560)
    };
    for (const QPointF &pos : posicionesMechas) {
        Mecha *m = new Mecha();
        m->setPos(pos.x(), pos.y());
        escena->addItem(m);
        m_mechas.append(m);
    }

    // Crear las rocas oscilantes (obstáculos entre el Mocho y las mechas)
    struct ParamRoca { double xCentro, yCentro, amplitud, omega, fase; };
    const QVector<ParamRoca> configRocas = {
        {400.0, 300.0, 80.0, 2.0, 0.0},
        {500.0, 350.0, 60.0, 1.5, 1.5},
        {450.0, 250.0, 70.0, 2.5, 3.0}
    };
    for (const ParamRoca &p : configRocas) {
        Roca *roca = new Roca(p.xCentro, p.yCentro, p.amplitud, p.omega, p.fase);
        escena->addItem(roca);
        m_rocas.append(roca);
    }

    // HUD: puntaje y tejos restantes
    m_textoPuntaje = escena->addText("Puntaje: 0");
    m_textoPuntaje->setPos(10, 5);
    m_textoPuntaje->setDefaultTextColor(QColor(255, 255, 255));
    m_textoPuntaje->setFont(QFont("Arial", 14, QFont::Bold));

    m_textoTejos = escena->addText("Tejos: 10");
    m_textoTejos->setPos(680, 5);
    m_textoTejos->setDefaultTextColor(QColor(255, 255, 255));
    m_textoTejos->setFont(QFont("Arial", 14, QFont::Bold));

    m_textoTiempo = escena->addText("Tiempo: 01:30");
    m_textoTiempo->setPos(330, 5);
    m_textoTiempo->setDefaultTextColor(QColor(255, 255, 255));
    m_textoTiempo->setFont(QFont("Arial", 14, QFont::Bold));

    m_textoViento = escena->addText("Viento: calma");
    m_textoViento->setDefaultTextColor(QColor(255, 255, 255));
    m_textoViento->setFont(QFont("Arial", 14, QFont::Bold));
    m_textoViento->setPos(320, 560);

    // Game loop: timer de física a ~60 FPS
    m_timerFisica = new QTimer(this);
    connect(m_timerFisica, &QTimer::timeout, this, &Juego::actualizarFisica);
    m_timerFisica->start(16);

    // Timer del nivel: cuenta regresiva de 90 segundos
    m_timerNivel = new QTimer(this);
    connect(m_timerNivel, &QTimer::timeout, this, &Juego::actualizarTiempo);
    m_timerNivel->start(1000);

    // Timer del viento: cambia la perturbación lateral cada 4 segundos
    m_timerViento = new QTimer(this);
    connect(m_timerViento, &QTimer::timeout, this, &Juego::cambiarViento);
    m_timerViento->start(4000);
}

Juego::~Juego() {}

void Juego::mousePressEvent(QMouseEvent *event) {
    if (m_juegoTerminado) {
        QMainWindow::mousePressEvent(event);
        return;
    }

    if (m_tejosRestantes <= 0 || m_tiempoRestante <= 0) {
        QMainWindow::mousePressEvent(event);
        return;
    }

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

    --m_tejosRestantes;
    actualizarHUD();

    QMainWindow::mousePressEvent(event);
}

void Juego::actualizarFisica() {
    // Actualizar todos los tejos y recoger los que se salieron de la escena
    for (int i = m_tejos.size() - 1; i >= 0; --i) {
        Tejo *tejo = m_tejos[i];
        tejo->actualizar(m_dtSegundos, m_vientoSolar);

        const double tx = tejo->x();
        const double ty = tejo->y();
        if (tx < 0 || tx > 800 || ty < 0 || ty > 600) {
            escena->removeItem(tejo);
            m_tejos.removeAt(i);
            delete tejo;
        }
    }

    // Actualizar las rocas oscilantes
    for (Roca *roca : m_rocas) {
        roca->actualizar(m_dtSegundos);
    }

    // Revisar impactos de tejos contra mechas y rocas
    detectarColisiones();

    verificarFinJuego();
}

void Juego::actualizarHUD() {
    if (m_textoPuntaje)
        m_textoPuntaje->setPlainText(QString("Puntaje: %1").arg(m_puntaje));
    if (m_textoTejos)
        m_textoTejos->setPlainText(QString("Tejos: %1").arg(m_tejosRestantes));
}

void Juego::actualizarTiempo() {
    if (m_tiempoRestante <= 0) return;
    --m_tiempoRestante;
    int minutos = m_tiempoRestante / 60;
    int segundos = m_tiempoRestante % 60;
    m_textoTiempo->setPlainText(QString("Tiempo: %1:%2")
        .arg(minutos, 2, 10, QChar('0'))
        .arg(segundos, 2, 10, QChar('0')));
    if (m_tiempoRestante == 0) {
        m_timerNivel->stop();
        m_timerViento->stop();
        if (!m_mechas.isEmpty()) {
            terminarJuego(false, "Se acabó el tiempo");
        }
    }
}

void Juego::cambiarViento() {
    // Genera un valor entre -150 y 150 px/s²
    m_vientoSolar = QRandomGenerator::global()->bounded(301) - 150;

    QString texto;
    if (m_vientoSolar < -20)
        texto = QString("Viento: ← %1").arg(static_cast<int>(-m_vientoSolar));
    else if (m_vientoSolar > 20)
        texto = QString("Viento: → %1").arg(static_cast<int>(m_vientoSolar));
    else
        texto = "Viento: calma";

    if (m_textoViento)
        m_textoViento->setPlainText(texto);
}

void Juego::detectarColisiones() {
    // Recorrer los tejos desde atrás para poder eliminarlos sin invalidar índices
    for (int i = m_tejos.size() - 1; i >= 0; --i) {
        Tejo *tejo = m_tejos[i];

        const QList<QGraphicsItem*> items = tejo->collidingItems();
        for (QGraphicsItem *item : items) {
            Mecha *mecha = dynamic_cast<Mecha*>(item);
            if (mecha && !mecha->explotada()) {
                // La mecha estalla: sumar puntos y eliminarla
                mecha->detonar();
                m_puntaje += mecha->puntos();
                actualizarHUD();

                escena->removeItem(mecha);
                m_mechas.removeOne(mecha);
                delete mecha;

                if (m_mechas.isEmpty()) {
                    terminarJuego(true, "¡Destruiste todas las mechas!");
                    return;
                }

                // El tejo también se destruye al impactar
                escena->removeItem(tejo);
                m_tejos.removeAt(i);
                delete tejo;

                // El tejo ya no existe: dejar de procesar sus colisiones
                break;
            }

            Roca *roca = dynamic_cast<Roca*>(item);
            if (roca) {
                // La roca destruye el tejo, pero no suma puntos ni desaparece
                escena->removeItem(tejo);
                m_tejos.removeAt(i);
                delete tejo;
                break;
            }
        }
    }
}

void Juego::verificarFinJuego() {
    if (m_juegoTerminado) return;
    if (m_tejosRestantes == 0 && m_tejos.isEmpty() && !m_mechas.isEmpty()) {
        terminarJuego(false, "Te quedaste sin tejos");
    }
}

void Juego::terminarJuego(bool victoria, const QString &mensaje) {
    if (m_juegoTerminado) return;
    m_juegoTerminado = true;

    m_timerFisica->stop();
    m_timerNivel->stop();
    m_timerViento->stop();

    m_textoResultado = new QGraphicsTextItem();
    if (victoria) {
        m_textoResultado->setPlainText("¡VICTORIA!");
        m_textoResultado->setDefaultTextColor(QColor(100, 255, 100));
        m_textoResultado->setFont(QFont("Arial", 48, QFont::Bold));
        m_textoResultado->setPos(240, 220);
    } else {
        m_textoResultado->setPlainText("JUEGO TERMINADO");
        m_textoResultado->setDefaultTextColor(QColor(255, 80, 80));
        m_textoResultado->setFont(QFont("Arial", 36, QFont::Bold));
        m_textoResultado->setPos(190, 220);
    }
    escena->addItem(m_textoResultado);

    m_textoSubresultado = new QGraphicsTextItem();
    m_textoSubresultado->setPlainText(
        QString("%1\nPuntaje final: %2").arg(mensaje).arg(m_puntaje)
    );
    m_textoSubresultado->setDefaultTextColor(QColor(255, 255, 255));
    m_textoSubresultado->setFont(QFont("Arial", 18));
    m_textoSubresultado->setPos(240, 310);
    escena->addItem(m_textoSubresultado);
}
