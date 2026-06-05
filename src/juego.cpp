#include "juego.h"
#include "tejo.h"
#include "mecha.h"
#include "roca.h"
#include "enemigo.h"
#include "comandante.h"

#include <QPainter>
#include <QPixmap>
#include <QBrush>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QUrl>
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
      m_tejosRestantes(20),
      m_tiempoRestante(90),
      m_timerNivel(nullptr),
      m_timerViento(nullptr),
      m_vientoSolar(0.0),
      m_textoPuntaje(nullptr),
      m_textoTejos(nullptr),
      m_textoTiempo(nullptr),
      m_textoViento(nullptr),
      m_juegoTerminado(false),
      m_textoResultado(nullptr),
      m_textoSubresultado(nullptr),
      m_textoReinicio(nullptr),
      m_nivelActual(1),
      m_comandante(nullptr),
      m_musicaFondo(nullptr),
      m_audioMusica(nullptr),
      m_efectoExplosion(nullptr),
      m_efectoEnemigoDestruido(nullptr),
      m_efectoDerrota(nullptr),
      m_efectoVictoria(nullptr),
      m_audioVictoria(nullptr)
{
    setWindowTitle("Tejo Cósmico");
    setMinimumSize(800, 600);
    showMaximized();

    // Crear la escena (el espacio donde viven los objetos del juego)
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 800, 600);
    QPixmap fondoNivel1(":/sprites/nivel1/superficie_lunar.png");
    if (!fondoNivel1.isNull()) {
        fondoNivel1 = fondoNivel1.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        escena->setBackgroundBrush(QBrush(fondoNivel1));
    } else {
        escena->setBackgroundBrush(QColor(20, 20, 50));
    }

    // Crear la vista (la cámara que muestra la escena)
    vista = new QGraphicsView(escena, this);
    vista->setRenderHint(QPainter::Antialiasing);
    vista->setRenderHint(QPainter::SmoothPixmapTransform, true);
    vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vista->fitInView(escena->sceneRect(), Qt::KeepAspectRatio);

    // Hacer que la vista ocupe toda la ventana
    setCentralWidget(vista);

    // Cargar el sprite del Mocho desde el recurso Qt
    QPixmap pixmapMocho(":/sprites/nivel1/Mocho.png");
    if (pixmapMocho.isNull()) {
        qWarning("No se pudo cargar el sprite del Mocho desde :/sprites/nivel1/Mocho.png");
    }

    // Crear el item con la imagen y colocarlo en el centro inferior de la escena
    m_mocho = new QGraphicsPixmapItem(pixmapMocho);
    m_mocho->setPos(150, 350);
    escena->addItem(m_mocho);

    // Crear las mechas (objetivos) del lado derecho de la escena
    const QVector<QPointF> posicionesMechas = {
        QPointF(700, 180),
        QPointF(650, 270),
        QPointF(720, 350),
        QPointF(660, 420),
        QPointF(700, 490)
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
        {350.0, 200.0, 60.0, 2.0, 0.0},
        {450.0, 280.0, 80.0, 1.5, 1.2},
        {550.0, 350.0, 70.0, 2.5, 2.5},
        {400.0, 420.0, 50.0, 1.8, 0.5},
        {520.0, 480.0, 90.0, 1.3, 3.0},
        {300.0, 350.0, 65.0, 2.2, 1.8}
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

    m_textoTejos = escena->addText("Tejos: 20");
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

    // Música de fondo en loop al 20% de volumen
    m_musicaFondo = new QMediaPlayer(this);
    m_audioMusica = new QAudioOutput(this);
    m_musicaFondo->setAudioOutput(m_audioMusica);
    m_musicaFondo->setSource(QUrl("qrc:/audio/musica.mp3"));
    m_audioMusica->setVolume(0.2);
    m_musicaFondo->setLoops(QMediaPlayer::Infinite);
    m_musicaFondo->play();

    // Efecto de sonido para explosiones
    m_efectoExplosion = new QSoundEffect(this);
    m_efectoExplosion->setSource(QUrl("qrc:/audio/explosion.wav"));
    m_efectoExplosion->setVolume(0.8);

    // Efecto de enemigo destruido (Nivel 2)
    m_efectoEnemigoDestruido = new QSoundEffect(this);
    m_efectoEnemigoDestruido->setSource(QUrl("qrc:/audio/enemigo_destruido.wav"));
    m_efectoEnemigoDestruido->setVolume(0.8);

    // Efecto de derrota
    m_efectoDerrota = new QSoundEffect(this);
    m_efectoDerrota->setSource(QUrl("qrc:/audio/derrota.wav"));
    m_efectoDerrota->setVolume(0.8);

    // Efecto de victoria (MP3, usa QMediaPlayer)
    m_efectoVictoria = new QMediaPlayer(this);
    m_audioVictoria = new QAudioOutput(this);
    m_efectoVictoria->setAudioOutput(m_audioVictoria);
    m_efectoVictoria->setSource(QUrl("qrc:/audio/victoria.mp3"));
    m_audioVictoria->setVolume(0.8);
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
    tejo->setPos(mochoX + 50, mochoY - 30);
    escena->addItem(tejo);
    m_tejos.append(tejo);
    tejo->lanzar(angulo, 700.0);

    if (m_comandante && m_nivelActual == 2)
        m_comandante->aprender(puntoEscena.x(), puntoEscena.y());

    --m_tejosRestantes;
    actualizarHUD();

    QMainWindow::mousePressEvent(event);
}

void Juego::keyPressEvent(QKeyEvent *event) {
    if (m_juegoTerminado && event->key() == Qt::Key_R) {
        reiniciarNivel();
    } else {
        QMainWindow::keyPressEvent(event);
    }
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

    // Actualizar enemigos y comprobar si alcanzaron al Mocho (Nivel 2)
    if (m_nivelActual == 2) {
        const double mochoX = m_mocho->x() + 25.0;
        const double mochoY = m_mocho->y() + 40.0;
        for (int i = m_enemigos.size() - 1; i >= 0; --i) {
            Enemigo *enemigo = m_enemigos[i];
            const bool esComandante = (m_comandante && enemigo == m_comandante);
            if (esComandante)
                m_comandante->actualizar(m_dtSegundos, mochoX, mochoY);
            else
                enemigo->actualizar(m_dtSegundos, mochoX, mochoY);

            const double halfSize = esComandante ? 25.0 : 15.0;
            const double cx = enemigo->x() + halfSize;
            const double cy = enemigo->y() + halfSize;
            const double ddx = cx - mochoX;
            const double ddy = cy - mochoY;
            if (qSqrt(ddx * ddx + ddy * ddy) < 30.0) {
                terminarJuego(false, "Un enemigo te alcanzó");
                return;
            }
        }
    }

    // Revisar impactos de tejos contra mechas, rocas y enemigos
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
        if (!m_mechas.isEmpty() || !m_enemigos.isEmpty()) {
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
                if (m_efectoExplosion)
                    m_efectoExplosion->play();

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

            ComandanteAlien *cmd = dynamic_cast<ComandanteAlien*>(item);
            if (cmd && !cmd->destruido()) {
                cmd->recibirImpacto();
                if (cmd->destruido()) {
                    m_puntaje += 500;
                    if (m_efectoEnemigoDestruido)
                        m_efectoEnemigoDestruido->play();
                    actualizarHUD();
                    escena->removeItem(cmd);
                    m_enemigos.removeOne(cmd);
                    m_comandante = nullptr;
                    delete cmd;
                    if (m_enemigos.isEmpty()) {
                        terminarJuego(true, "¡Destruiste a todos los invasores!");
                        return;
                    }
                }
                escena->removeItem(tejo);
                m_tejos.removeAt(i);
                delete tejo;
                break;
            }

            Enemigo *enemigo = dynamic_cast<Enemigo*>(item);
            if (enemigo && !enemigo->destruido()) {
                enemigo->recibirImpacto();
                if (enemigo->destruido()) {
                    m_puntaje += 200;
                    if (m_efectoEnemigoDestruido)
                        m_efectoEnemigoDestruido->play();
                    actualizarHUD();
                    escena->removeItem(enemigo);
                    m_enemigos.removeOne(enemigo);
                    delete enemigo;
                    if (m_enemigos.isEmpty()) {
                        terminarJuego(true, "¡Destruiste a todos los invasores!");
                        return;
                    }
                }
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
    if (m_nivelActual == 1 && m_tejosRestantes == 0 && m_tejos.isEmpty() && !m_mechas.isEmpty()) {
        terminarJuego(false, "Te quedaste sin tejos");
    } else if (m_nivelActual == 2 && m_tejosRestantes == 0 && m_tejos.isEmpty() && !m_enemigos.isEmpty()) {
        terminarJuego(false, "Te quedaste sin tejos");
    }
}

void Juego::terminarJuego(bool victoria, const QString &mensaje) {
    if (m_juegoTerminado) return;
    m_juegoTerminado = true;

    m_timerFisica->stop();
    m_timerNivel->stop();
    m_timerViento->stop();

    if (victoria && m_nivelActual == 1) {
        m_textoResultado = new QGraphicsTextItem();
        m_textoResultado->setPlainText("¡Pasando al Nivel 2!");
        m_textoResultado->setDefaultTextColor(QColor(100, 255, 100));
        m_textoResultado->setFont(QFont("Arial", 32, QFont::Bold));
        m_textoResultado->setPos(185, 260);
        escena->addItem(m_textoResultado);
        QTimer::singleShot(2500, this, &Juego::cargarNivel2);
        return;
    }

    m_textoResultado = new QGraphicsTextItem();
    if (victoria) {
        if (m_efectoVictoria)
            m_efectoVictoria->play();
        m_textoResultado->setPlainText("¡VICTORIA!");
        m_textoResultado->setDefaultTextColor(QColor(100, 255, 100));
        m_textoResultado->setFont(QFont("Arial", 48, QFont::Bold));
        m_textoResultado->setPos(240, 220);
    } else {
        if (m_efectoDerrota)
            m_efectoDerrota->play();
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

    m_textoReinicio = new QGraphicsTextItem();
    m_textoReinicio->setPlainText("Presiona R para reiniciar");
    m_textoReinicio->setDefaultTextColor(QColor(200, 200, 200));
    m_textoReinicio->setFont(QFont("Arial", 16));
    m_textoReinicio->setPos(255, 400);
    escena->addItem(m_textoReinicio);
}

void Juego::limpiarEscena() {
    for (Mecha *mecha : m_mechas) {
        escena->removeItem(mecha);
        delete mecha;
    }
    m_mechas.clear();

    for (Roca *roca : m_rocas) {
        escena->removeItem(roca);
        delete roca;
    }
    m_rocas.clear();

    for (Tejo *tejo : m_tejos) {
        escena->removeItem(tejo);
        delete tejo;
    }
    m_tejos.clear();

    for (Enemigo *enemigo : m_enemigos) {
        escena->removeItem(enemigo);
        delete enemigo;
    }
    m_enemigos.clear();
    m_comandante = nullptr;

    auto limpiarTexto = [this](QGraphicsTextItem *&texto) {
        if (texto) {
            escena->removeItem(texto);
            delete texto;
            texto = nullptr;
        }
    };
    limpiarTexto(m_textoPuntaje);
    limpiarTexto(m_textoTejos);
    limpiarTexto(m_textoTiempo);
    limpiarTexto(m_textoViento);
    limpiarTexto(m_textoResultado);
    limpiarTexto(m_textoSubresultado);
    limpiarTexto(m_textoReinicio);
}

void Juego::cargarNivel2() {
    QPixmap fondoNivel2(":/sprites/nivel2/anillos_de_saturno.png");
    if (!fondoNivel2.isNull()) {
        fondoNivel2 = fondoNivel2.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        escena->setBackgroundBrush(QBrush(fondoNivel2));
    }

    m_nivelActual = 2;
    m_juegoTerminado = false;
    limpiarEscena();

    m_puntaje = 0;
    m_tejosRestantes = 15;
    m_tiempoRestante = 60;

    const int mochoWidth  = m_mocho->pixmap().width();
    const int mochoHeight = m_mocho->pixmap().height();
    m_mocho->setPos(400 - mochoWidth / 2, 300 - mochoHeight / 2);

    m_textoPuntaje = escena->addText("Puntaje: 0");
    m_textoPuntaje->setPos(10, 5);
    m_textoPuntaje->setDefaultTextColor(QColor(255, 255, 255));
    m_textoPuntaje->setFont(QFont("Arial", 14, QFont::Bold));

    m_textoTejos = escena->addText("Tejos: 15");
    m_textoTejos->setPos(680, 5);
    m_textoTejos->setDefaultTextColor(QColor(255, 255, 255));
    m_textoTejos->setFont(QFont("Arial", 14, QFont::Bold));

    m_textoTiempo = escena->addText("Tiempo: 01:00");
    m_textoTiempo->setPos(330, 5);
    m_textoTiempo->setDefaultTextColor(QColor(255, 255, 255));
    m_textoTiempo->setFont(QFont("Arial", 14, QFont::Bold));

    m_textoViento = escena->addText("Viento: calma");
    m_textoViento->setDefaultTextColor(QColor(255, 255, 255));
    m_textoViento->setFont(QFont("Arial", 14, QFont::Bold));
    m_textoViento->setPos(320, 560);

    const QVector<QPointF> posicionesEnemigos = {
        QPointF(50,  50),
        QPointF(750, 50),
        QPointF(50,  550),
        QPointF(750, 550)
    };
    for (const QPointF &pos : posicionesEnemigos) {
        Enemigo *e = new Enemigo(pos.x(), pos.y());
        escena->addItem(e);
        m_enemigos.append(e);
    }

    m_comandante = new ComandanteAlien(100, 300);
    escena->addItem(m_comandante);
    m_enemigos.append(m_comandante);

    m_timerFisica->start(16);
    m_timerNivel->start(1000);
    m_timerViento->start(4000);
}

void Juego::reiniciarNivel() {
    int nivelARecargar = m_nivelActual;
    limpiarEscena();
    m_juegoTerminado = false;
    m_puntaje = 0;

    if (nivelARecargar == 1) {
        m_tejosRestantes = 20;
        m_tiempoRestante = 90;
        m_nivelActual = 1;

        m_mocho->setPos(150, 350);

        m_textoPuntaje = escena->addText("Puntaje: 0");
        m_textoPuntaje->setPos(10, 5);
        m_textoPuntaje->setDefaultTextColor(QColor(255, 255, 255));
        m_textoPuntaje->setFont(QFont("Arial", 14, QFont::Bold));

        m_textoTejos = escena->addText("Tejos: 20");
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

        const QVector<QPointF> posicionesMechas = {
            QPointF(700, 180), QPointF(650, 270), QPointF(720, 350),
            QPointF(660, 420), QPointF(700, 490)
        };
        for (const QPointF &pos : posicionesMechas) {
            Mecha *m = new Mecha();
            m->setPos(pos.x(), pos.y());
            escena->addItem(m);
            m_mechas.append(m);
        }

        struct ParamRoca { double xCentro, yCentro, amplitud, omega, fase; };
        const QVector<ParamRoca> configRocas = {
            {350.0, 200.0, 60.0, 2.0, 0.0},
            {450.0, 280.0, 80.0, 1.5, 1.2},
            {550.0, 350.0, 70.0, 2.5, 2.5},
            {400.0, 420.0, 50.0, 1.8, 0.5},
            {520.0, 480.0, 90.0, 1.3, 3.0},
            {300.0, 350.0, 65.0, 2.2, 1.8}
        };
        for (const ParamRoca &p : configRocas) {
            Roca *roca = new Roca(p.xCentro, p.yCentro, p.amplitud, p.omega, p.fase);
            escena->addItem(roca);
            m_rocas.append(roca);
        }

        QPixmap fondoNivel1(":/sprites/nivel1/superficie_lunar.png");
        if (!fondoNivel1.isNull()) {
            fondoNivel1 = fondoNivel1.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            escena->setBackgroundBrush(QBrush(fondoNivel1));
        }

        m_timerFisica->start(16);
        m_timerNivel->start(1000);
        m_timerViento->start(4000);
    } else {
        cargarNivel2();
    }
}

void Juego::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    if (vista && escena) {
        vista->fitInView(escena->sceneRect(), Qt::KeepAspectRatio);
    }
}
