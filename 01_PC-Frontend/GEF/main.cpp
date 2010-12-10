#include <QtGui/QApplication>
#include "mainwindow.h"

//#ifndef RUN_TESTS

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

//#endif
