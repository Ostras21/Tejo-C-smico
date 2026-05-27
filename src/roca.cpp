#include "roca.h"
#include <QPainter>
#include <QPixmap>
#include <cmath>

Roca::Roca(double xCentro, double yCentro, double amplitud, double omega, double fase,
           QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent),
      m_yCentro(yCentro),
      m_amplitud(amplitud),
      m_omega(omega),
      m_fase(fase),
      m_tiempo(0.0)
{
    // Generar sprite de roca espacial con QPainter
    QPixmap pixmap(50, 50);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // Cuerpo principal: elipse irregular gris-marrón
    painter.setBrush(QColor(100, 70, 60));
    painter.setPen(QPen(QColor(60, 40, 35), 1));
    painter.drawEllipse(5, 7, 40, 36);

    // Cráter 1: elipse oscura interior
    painter.setBrush(QColor(65, 45, 38));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(12, 13, 14, 10);

    // Cráter 2: elipse más oscura
    painter.setBrush(QColor(55, 38, 30));
    painter.drawEllipse(26, 24, 10, 8);

    painter.end();

    setPixmap(pixmap);
    setTransformOriginPoint(25, 25);
    setPos(xCentro - 25, yCentro - 25);
}

void Roca::actualizar(double dt) {
    m_tiempo += dt;
    double nuevaY = m_yCentro + m_amplitud * std::sin(m_omega * m_tiempo + m_fase);
    setY(nuevaY - 25);
}
