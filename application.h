#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QApplication>
#include <QFileOpenEvent>
#include "mainwindow.h"

class MainWindow;

class Application : public QApplication
{
public:
    Application(int &argc, char **argv);

    bool event(QEvent *event);

    MainWindow* firstMainWindow = NULL;
};

#endif // APPLICATION_H
