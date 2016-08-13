#include "mainwindow.h"
#include <QApplication>
#include <QList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    #if (defined Q_OS_MACX)
        a.setQuitOnLastWindowClosed(false);
    #endif

    QString FileToOpen = "";

    QStringList args = a.arguments();
    args.removeFirst();
    for (QString arg : args) {
        FileToOpen = arg;
    }

    MainWindow w(FileToOpen);
    w.show();

    return a.exec();
}
