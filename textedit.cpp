#include "textedit.h"

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent)
{

}

void TextEdit::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        emit EnterPressed();
    }
    QTextEdit::keyPressEvent(event);
}

void TextEdit::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasHtml() || event->mimeData()->hasImage() || event->mimeData()->hasText() || event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void TextEdit::dropEvent(QDropEvent *event) {
    QTextCursor cursor = this->cursorForPosition(event->pos());

    if (event->mimeData()->hasHtml()) {
        cursor.insertHtml(event->mimeData()->html());
    } else if (event->mimeData()->hasImage()) {
        cursor.insertImage(event->mimeData()->imageData().value<QImage>());
    } else if (event->mimeData()->hasText()) {
        cursor.insertText(event->mimeData()->text());
    }
}
