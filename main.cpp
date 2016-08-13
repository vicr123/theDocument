#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    #if (defined Q_OS_MACX)
        a.setQuitOnLastWindowClosed(false);
    #endif

    MainWindow w;
    w.show();

    return a.exec();
}
