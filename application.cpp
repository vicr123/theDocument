#include "application.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
    #ifdef Q_OS_MAC
        setupDockClickHandler();
    #endif

}

bool Application::event(QEvent *event) {
    if (event->type() == QEvent::FileOpen) {
        QFileOpenEvent* fileEvent = (QFileOpenEvent*) event;
        MainWindow* w = new MainWindow(fileEvent->file());
        w->show();

        if (firstMainWindow != NULL) {
            if (firstMainWindow->allowClose()) {
                firstMainWindow->close();
                firstMainWindow = NULL;
            }
        }
        return true;
    } else {
        return QApplication::event(event);
    }
}

#ifdef Q_OS_MAC
void setupDockClickHandler() {
    Class cls = objc_getClass("NSApplication");
    objc_object *appInst = objc_msgSend((objc_object*)cls, sel_registerName("sharedApplication"));

    if (appInst != NULL) {
        objc_object* delegate = objc_msgSend(appInst, sel_registerName("delegate"));
        Class delClass = (Class)objc_msgSend(delegate,  sel_registerName("class"));
        SEL shouldHandle = sel_registerName("applicationShouldHandleReopen:hasVisibleWindows:");
        if (class_getInstanceMethod(delClass, shouldHandle)) {
            if (class_replaceMethod(delClass, shouldHandle, (IMP)dockClickHandler, "B@:")) {
                qDebug() << "Registered dock click handler (replaced original method)";
            } else {
                qWarning() << "Failed to replace method for dock click handler";
            }
        } else {
            if (class_addMethod(delClass, shouldHandle, (IMP)dockClickHandler,"B@:")) {
                qDebug() << "Registered dock click handler";
            } else {
                qWarning() << "Failed to register dock click handler";
            }
        }
    }
}

bool dockClickHandler(id self,SEL _cmd,...) {
    Q_UNUSED(self)
    Q_UNUSED(_cmd)

    QWidgetList widgets = QApplication::topLevelWidgets();
    bool openNewWindow = true;
    for (QWidget* widget : widgets) {
        QMainWindow* mainWindow = dynamic_cast<MainWindow*>(widget);
        if (mainWindow != NULL) {
            if (mainWindow->isVisible()) {
                openNewWindow = false;
                if (mainWindow->isMinimized()) {
                    mainWindow->showNormal();
                }
                mainWindow->activateWindow();
                mainWindow->raise();
                break;
            }
        }
    }

    if (openNewWindow) {
        MainWindow* w = new MainWindow();
        w->show();
    }

    // Return false to suppress the default OS X actions
    return false;
}
#endif
