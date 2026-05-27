#include "qconfigwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QConfigWidget w;
    w.showMaximized();
    return QApplication::exec();
}
