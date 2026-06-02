#include "comandante.h"
#include "tejo.h"

#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <cmath>

ComandanteAlien::ComandanteAlien(double x, double y, QGraphicsItem *parent)
    : Enemigo(x, y, parent),
      m_cuadranteObjetivo(0),
      m_velocidad(50.0),
      m_golpesRecibidos(0)
{
    m_conteoTejos[0] = 0;
    m_conteoTejos[1] = 0;
    m_conteoTejos[2] = 0;
    m_conteoTejos[3] = 0;

    QPixmap pixmap(50, 50);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(QColor(100, 20, 150), 2));
    painter.drawLine(18, 8, 12, 0);
    painter.drawLine(32, 8, 38, 0);

    painter.setBrush(QColor(80, 20, 120));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(3, 10, 44, 40);

    painter.setBrush(QColor(150, 50, 200));
    painter.drawEllipse(6, 13, 38, 34);

    painter.setBrush(QColor(0, 0, 0));
    painter.drawEllipse(11, 18, 10, 10);
    painter.drawEllipse(29, 18, 10, 10);

    painter.end();

    setPixmap(pixmap);
    setTransformOriginPoint(25, 25);
    setPos(x - 25, y - 25);
}

int ComandanteAlien::cuadranteDe(double x, double y) const {
    if (x < 400.0 && y < 300.0) return 0;
    if (x >= 400.0 && y < 300.0) return 1;
    if (x < 400.0 && y >= 300.0) return 2;
    return 3;
}

void ComandanteAlien::percibir(const QList<Tejo*> & /*tejos*/) {
    razonar();
}

int ComandanteAlien::razonar() {
    int minIdx = 0;
    for (int i = 1; i < 4; ++i) {
        if (m_conteoTejos[i] < m_conteoTejos[minIdx])
            minIdx = i;
    }
    m_cuadranteObjetivo = minIdx;
    return minIdx;
}

void ComandanteAlien::actuar(double dt) {
    static const double centrosX[4] = {200.0, 600.0, 200.0, 600.0};
    static const double centrosY[4] = {150.0, 150.0, 450.0, 450.0};

    const double tx = centrosX[m_cuadranteObjetivo];
    const double ty = centrosY[m_cuadranteObjetivo];
    const double cx = x() + 25.0;
    const double cy = y() + 25.0;
    const double dx = tx - cx;
    const double dy = ty - cy;
    const double mag = std::sqrt(dx * dx + dy * dy);

    if (mag < 2.0) return;

    setPos(x() + (dx / mag) * m_velocidad * dt,
           y() + (dy / mag) * m_velocidad * dt);
}

void ComandanteAlien::aprender(double x, double y) {
    m_conteoTejos[cuadranteDe(x, y)]++;
}

void ComandanteAlien::actualizar(double dt, double /*mochoX*/, double /*mochoY*/) {
    razonar();
    actuar(dt);
}

void ComandanteAlien::recibirImpacto() {
    ++m_golpesRecibidos;
}

bool ComandanteAlien::destruido() const {
    return m_golpesRecibidos >= 3;
}
