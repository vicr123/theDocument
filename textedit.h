#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEdit(QWidget *parent = 0);

signals:
    void EnterPressed();

public slots:

private:
    void keyPressEvent(QKeyEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // TEXTEDIT_H
