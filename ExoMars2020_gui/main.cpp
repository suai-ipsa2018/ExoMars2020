#include "exomars2020_gui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ExoMars2020_gui w;
    w.show();

    return a.exec();
}
