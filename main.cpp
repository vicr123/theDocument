#include "mainwindow.h"
#include "application.h"
#include <QList>

Application* a;

int main(int argc, char *argv[])
{
    a = new Application(argc, argv);

    #ifdef Q_OS_MACX
        a->setQuitOnLastWindowClosed(false);
    #endif

    MainWindow w;
    a->firstMainWindow = &w;
    w.show();

    return a->exec();
}
