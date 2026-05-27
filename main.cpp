#include "qconfigwidget.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/my.ico"));
    QConfigWidget w;
    w.showMaximized();
    return QApplication::exec();
}
