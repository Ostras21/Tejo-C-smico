#ifndef MECHA_H
#define MECHA_H

#include <QGraphicsPixmapItem>

class Mecha : public QGraphicsPixmapItem {
public:
    Mecha(QGraphicsItem *parent = nullptr);

    void detonar();

    bool   explotada() const;
    int    puntos() const;
    double radioExplosion() const;

private:
    bool   m_explotada;
    int    m_puntos;
    double m_radioExplosion;
};

#endif // MECHA_H
