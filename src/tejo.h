#ifndef TEJO_H
#define TEJO_H

#include <QGraphicsPixmapItem>

class Tejo : public QGraphicsPixmapItem {
public:
    Tejo(QGraphicsItem *parent = nullptr);

    void lanzar(double anguloRad, double fuerza);
    void actualizar(double dt);
    bool enVuelo() const { return m_enVuelo; }

private:
    double m_vx;
    double m_vy;
    double m_gravedad;
    bool   m_enVuelo;
};

#endif // TEJO_H
