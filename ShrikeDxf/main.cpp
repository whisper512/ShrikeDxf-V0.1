#include "ShrikeDxf.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ShrikeDxf window;
    window.show();
    return app.exec();
}
