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
    // Dibujar un círculo gris oscuro con borde negro como sprite provisional.
    const int radio = 15;
    const int lado  = radio * 2 + 2; // +2 para que el borde no se recorte

    QPixmap pix(lado, lado);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(QColor(80, 80, 80)));   // gris oscuro
    painter.setPen(QPen(Qt::black, 2));
    painter.drawEllipse(1, 1, radio * 2, radio * 2);
    painter.end();

    setPixmap(pix);
    setTransformOriginPoint(lado / 2.0, lado / 2.0);
}

void Tejo::lanzar(double anguloRad, double fuerza) {
    m_vx = fuerza * std::cos(anguloRad);
    m_vy = -fuerza * std::sin(anguloRad); // Y crece hacia abajo en Qt
    m_enVuelo = true;
}

void Tejo::actualizar(double dt) {
    if (!m_enVuelo) return;

    m_vy += m_gravedad * dt;

    const double nuevaX = x() + m_vx * dt;
    const double nuevaY = y() + m_vy * dt;
    setPos(nuevaX, nuevaY);
}
