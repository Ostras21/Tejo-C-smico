#include <QApplication>
#include "juego.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Juego ventana;
    ventana.show();
    return app.exec();
}
