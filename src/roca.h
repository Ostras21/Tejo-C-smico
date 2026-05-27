#ifndef ROCA_H
#define ROCA_H

#include <QGraphicsPixmapItem>

class Roca : public QGraphicsPixmapItem {
public:
    Roca(double xCentro, double yCentro, double amplitud, double omega, double fase,
         QGraphicsItem *parent = nullptr);
    void actualizar(double dt);

private:
    double m_yCentro;
    double m_amplitud;
    double m_omega;
    double m_fase;
    double m_tiempo;
};

#endif // ROCA_H
