#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextDocument>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTextCursor>
#include <QInputDialog>
#include "application.h"

class Application;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString OpenWithFile = "", QWidget *parent = 0);
    ~MainWindow();

    bool allowClose();

private slots:
    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_actionRedo_triggered();

    void on_actionUndo_triggered();

    void on_documentView_cursorPositionChanged();

    void on_documentView_currentCharFormatChanged(const QTextCharFormat &format);

    void on_actionBold_triggered(bool arg1);

    void on_actionItalic_triggered(bool arg1);

    void on_actionUnderline_triggered(bool arg1);

    void on_actionStrikeout_triggered(bool arg1);

    void on_actionPage_Break_triggered();

    void on_actionInsert_Table_triggered();

    void on_fontFamily_currentFontChanged(const QFont &f);

    void on_actionSave_As_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_fontSize_valueChanged(double arg1);

    void on_actionClose_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;

    bool closeOnceSaved = false;

    void closeEvent(QCloseEvent *event);
    QMessageBox::StandardButton showMessageBox(QString title, QString text, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::Ok);

    QTextDocument* currentDocument;
    QTextCursor currentCursor;
    QString currentFile = "";
    bool hasHadEdits = false;
};

#endif // MAINWINDOW_H
