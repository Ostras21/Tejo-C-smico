#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <QGraphicsPixmapItem>

class Enemigo : public QGraphicsPixmapItem {
public:
    Enemigo(double x, double y, QGraphicsItem *parent = nullptr);

    void actualizar(double dt, double mochoX, double mochoY);
    void recibirImpacto();
    bool destruido() const;

private:
    double m_vx;
    double m_vy;
    int    m_vida;
    double m_amplitudZigzag;
    double m_tiempoZigzag;
    bool   m_destruido;
};

#endif // ENEMIGO_H
