#include "mecha.h"

#include <QPainter>
#include <QPixmap>
#include <QPolygon>

Mecha::Mecha(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent),
      m_explotada(false),
      m_puntos(100),
      m_radioExplosion(30.0)
{
    // Dibujar el aspecto de la mecha sobre un pixmap transparente
    QPixmap pixmap(30, 40);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // Triángulo amarillo-naranja apuntando hacia arriba
    QPolygon triangulo;
    triangulo << QPoint(15, 5) << QPoint(5, 35) << QPoint(25, 35);

    painter.setBrush(QBrush(QColor(255, 165, 0)));
    painter.setPen(QPen(Qt::black, 1.5));
    painter.drawPolygon(triangulo);

    // Chispa roja brillante en la punta
    painter.setBrush(QBrush(QColor(255, 0, 0)));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(15, 5), 3, 3);

    painter.end();

    setPixmap(pixmap);
    setTransformOriginPoint(15, 20);  // centro de la mecha
}

void Mecha::detonar() {
    m_explotada = true;
}

bool Mecha::explotada() const {
    return m_explotada;
}

int Mecha::puntos() const {
    return m_puntos;
}

double Mecha::radioExplosion() const {
    return m_radioExplosion;
}
