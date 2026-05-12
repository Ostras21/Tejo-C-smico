#ifndef JUEGO_H
#define JUEGO_H

#include <QMainWindow>

class Juego : public QMainWindow {
    Q_OBJECT
public:
    Juego(QWidget *parent = nullptr);
    ~Juego();
};

#endif // JUEGO_H
