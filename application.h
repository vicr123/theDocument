#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QApplication>
#include <QFileOpenEvent>
#include <QWindow>
#include "mainwindow.h"

#ifdef Q_OS_MAC
#include <objc/objc.h>
#include <objc/message.h>
void setupDockClickHandler();
bool dockClickHandler(id self,SEL _cmd,...);
#endif

class MainWindow;

class Application : public QApplication
{
public:
    Application(int &argc, char **argv);

    bool event(QEvent *event);

    MainWindow* firstMainWindow = NULL;
};

#endif // APPLICATION_H
