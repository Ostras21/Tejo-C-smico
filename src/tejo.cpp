#include "tejo.h"
#include <QPainter>
#include <QPixmap>
#include <QBrush>
#include <QPen>
#include <cmath>

Tejo::Tejo(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent),
      m_vx(0.0),
      m_vy(0.0),
      m_gravedad(800.0),
      m_enVuelo(false)
{
    QPixmap pixmap(":/sprites/nivel1/tejo.png");
    if (pixmap.isNull()) {
        const int radio = 39;
        const int lado  = radio * 2 + 2;  // 80

        QPixmap pix(lado, lado);
        pix.fill(Qt::transparent);

        QPainter painter(&pix);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QBrush(QColor(80, 80, 80)));
        painter.setPen(QPen(Qt::black, 2));
        painter.drawEllipse(1, 1, radio * 2, radio * 2);
        painter.end();

        setPixmap(pix);
    } else {
        pixmap = pixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(pixmap);
    }
    setTransformOriginPoint(40, 40);
}

void Tejo::lanzar(double anguloRad, double fuerza) {
    m_vx = fuerza * std::cos(anguloRad);
    m_vy = -fuerza * std::sin(anguloRad); // Y crece hacia abajo en Qt
    m_enVuelo = true;
}

void Tejo::actualizar(double dt, double vientoSolar) {
    if (!m_enVuelo) return;

    m_vx += vientoSolar * dt;
    m_vy += m_gravedad * dt;

    const double nuevaX = x() + m_vx * dt;
    const double nuevaY = y() + m_vy * dt;
    setPos(nuevaX, nuevaY);
}
