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
    QPixmap pixmap(":/sprites/nivel1/roca.png");
    if (pixmap.isNull()) {
        QPixmap pix(100, 100);
        pix.fill(Qt::transparent);

        QPainter painter(&pix);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setBrush(QColor(100, 70, 60));
        painter.setPen(QPen(QColor(60, 40, 35), 1));
        painter.drawEllipse(9, 13, 82, 73);

        painter.setBrush(QColor(65, 45, 38));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(24, 25, 29, 21);

        painter.setBrush(QColor(55, 38, 30));
        painter.drawEllipse(52, 49, 21, 16);

        painter.end();

        setPixmap(pix);
    } else {
        pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(pixmap);
    }
    setTransformOriginPoint(50, 50);
    setPos(xCentro - 50, yCentro - 50);
}

void Roca::actualizar(double dt) {
    m_tiempo += dt;
    double nuevaY = m_yCentro + m_amplitud * std::sin(m_omega * m_tiempo + m_fase);
    setY(nuevaY - 50);
}
