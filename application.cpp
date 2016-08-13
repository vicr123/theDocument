#include "application.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{

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
