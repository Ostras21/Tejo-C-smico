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
    QPixmap pixmap(":/sprites/nivel1/mecha.png");
    if (pixmap.isNull()) {
        QPixmap pix(95, 120);
        pix.fill(Qt::transparent);

        QPainter painter(&pix);
        painter.setRenderHint(QPainter::Antialiasing);

        QPolygon triangulo;
        triangulo << QPoint(47, 9) << QPoint(9, 111) << QPoint(86, 111);

        painter.setBrush(QBrush(QColor(255, 165, 0)));
        painter.setPen(QPen(Qt::black, 1.5));
        painter.drawPolygon(triangulo);

        painter.setBrush(QBrush(QColor(255, 0, 0)));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPointF(47, 9), 8, 8);

        painter.end();

        setPixmap(pix);
    } else {
        pixmap = pixmap.scaled(95, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(pixmap);
    }
    setTransformOriginPoint(47, 60);
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
