#include "enemigo.h"

#include <QPainter>
#include <QPixmap>
#include <cmath>

Enemigo::Enemigo(double x, double y, QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent),
      m_vx(0.0),
      m_vy(0.0),
      m_vida(1),
      m_amplitudZigzag(30.0),
      m_tiempoZigzag(0.0),
      m_destruido(false)
{
    QPixmap pixmap(":/sprites/nivel2/enemigo.png");
    if (pixmap.isNull()) {
        QPixmap pix(85, 85);
        pix.fill(Qt::transparent);

        QPainter painter(&pix);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setBrush(QColor(100, 20, 20));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(7, 7, 71, 71);

        painter.setBrush(QColor(180, 30, 30));
        painter.drawEllipse(13, 13, 59, 59);

        painter.setBrush(QColor(0, 0, 0));
        painter.drawEllipse(18, 17, 17, 17);
        painter.drawEllipse(50, 17, 17, 17);

        painter.end();

        setPixmap(pix);
    } else {
        pixmap = pixmap.scaled(85, 85, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        setPixmap(pixmap);
    }
    setTransformOriginPoint(42, 42);
    setPos(x - 42, y - 42);
}

void Enemigo::actualizar(double dt, double mochoX, double mochoY) {
    const double cx = x() + 15.0;
    const double cy = y() + 15.0;
    const double dx = mochoX - cx;
    const double dy = mochoY - cy;
    const double mag = std::sqrt(dx * dx + dy * dy);
    if (mag < 1.0) return;

    const double ndx = dx / mag;
    const double ndy = dy / mag;

    m_tiempoZigzag += dt;
    const double offset = std::sin(m_tiempoZigzag * 3.0) * m_amplitudZigzag;

    const double pdx = -ndy;
    const double pdy =  ndx;

    const double newX = x() + (ndx * 40.0 + pdx * offset) * dt;
    const double newY = y() + (ndy * 40.0 + pdy * offset) * dt;
    setPos(newX, newY);
}

void Enemigo::recibirImpacto() {
    --m_vida;
    if (m_vida <= 0) m_destruido = true;
}

bool Enemigo::destruido() const {
    return m_destruido;
}
