#ifndef COMANDANTE_H
#define COMANDANTE_H

#include "enemigo.h"
#include <QList>

class Tejo;

class ComandanteAlien : public Enemigo {
public:
    ComandanteAlien(double x, double y, QGraphicsItem *parent = nullptr);

    void percibir(const QList<Tejo*> &tejos);
    int  razonar();
    void actuar(double dt);
    void aprender(double x, double y);

    void actualizar(double dt, double mochoX, double mochoY);
    void recibirImpacto();
    bool destruido() const;

private:
    int    m_conteoTejos[4];
    int    m_cuadranteObjetivo;
    double m_velocidad;
    int    m_golpesRecibidos;

    int cuadranteDe(double x, double y) const;
};

#endif // COMANDANTE_H
