#include "mainwindow.h"
#include "ui_mainwindow.h"

extern Application* a;

MainWindow::MainWindow(QString OpenWithFile, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->actionNew->setShortcut(QKeySequence::New);
    ui->actionOpen->setShortcut(QKeySequence::Open);
    ui->actionSave->setShortcut(QKeySequence::Save);
    ui->actionUndo->setShortcut(QKeySequence::Undo);
    ui->actionRedo->setShortcut(QKeySequence::Redo);
    ui->actionBold->setShortcut(QKeySequence::Bold);
    ui->actionItalic->setShortcut(QKeySequence::Italic);
    ui->actionUnderline->setShortcut(QKeySequence::Underline);
    ui->actionCopy->setShortcut(QKeySequence::Copy);
    ui->actionCut->setShortcut(QKeySequence::Cut);
    ui->actionPaste->setShortcut(QKeySequence::Paste);

    ui->mainToolBar->insertWidget(ui->actionBold, ui->fontInfoFrame);

    currentDocument = ui->documentView->document();
    currentDocument->setDefaultStyleSheet("span.pagebrk:before {\n"
                                          "content: \"---PAGE BREAK---\";"
                                          "}");

    //ui->documentView->setDocument(currentDocument);

    currentCursor = QTextCursor(currentDocument);

    connect(currentDocument, &QTextDocument::modificationChanged, [=](bool modified) {
        this->setWindowModified(modified);
        if (modified) {
            hasHadEdits = true;
        }
    });
    connect(currentDocument, &QTextDocument::undoAvailable, [=](bool available) {
        ui->actionUndo->setEnabled(available);
    });
    connect(currentDocument, &QTextDocument::redoAvailable, [=](bool available) {
        ui->actionRedo->setEnabled(available);
    });

    on_documentView_currentCharFormatChanged(ui->documentView->currentCharFormat());

    if (OpenWithFile != "") {
        QFile file(OpenWithFile);
        file.open(QFile::ReadOnly);
        currentDocument->setHtml(QString(file.readAll()));
        file.close();

        currentFile = OpenWithFile;
        this->setWindowTitle("");
        this->setWindowFilePath(currentFile);
        currentDocument->setModified(false);
    }
}

MainWindow::~MainWindow()
{
    if (a->firstMainWindow == this) {
        a->firstMainWindow = NULL;
    }
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (currentDocument->isModified()) { //Ask if the user wants to save
        QMessageBox::StandardButton button = showMessageBox("Save Changes?", "If you don't save the changes that you made to this document, they will be lost.", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Cancel);

        if (button == QMessageBox::Cancel) {
            event->ignore();
        } else if (button == QMessageBox::Save) {
            closeOnceSaved = true;
            ui->actionSave->trigger();
            event->ignore();
        } else if (button == QMessageBox::Discard) {
            currentDocument->setModified(false);
            event->accept();
        }
    } else {
        event->accept();
    }
}

QMessageBox::StandardButton MainWindow::showMessageBox(QString title, QString text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton) {
    QMessageBox* box = new QMessageBox(this);
    #if (defined Q_OS_MACX)
        box->setText(title);
        box->setInformativeText(text);
    #else
        box->setWindowTitle(title);
        box->setText(text);
    #endif
    box->setStandardButtons(buttons);
    box->setDefaultButton(defaultButton);
    box->setWindowFlags(Qt::Sheet);
    box->setIcon(QMessageBox::Warning);

    int result = -1;

    connect(box, &QMessageBox::finished, [=, &result](int returnedResult) {
        result = returnedResult;
    });

    box->show();

    while (result == -1) {
        QApplication::processEvents();
    }

    return (QMessageBox::StandardButton) result;
}

void MainWindow::on_actionSave_triggered()
{
    if (currentFile == "") {
        on_actionSave_As_triggered();
    } else {
        QFile file(currentFile);
        file.open(QFile::WriteOnly);
        file.write(currentDocument->toHtml().toUtf8());
        file.close();
        currentDocument->setModified(false);

        if (closeOnceSaved) {
            this->close();
        }
    }
}


void MainWindow::on_actionSave_As_triggered()
{
    QStringList filters;
    filters << "theDocument HTML File (*.tdh)"
            << "Web Page (*.html)"
            << "OpenDocument Text (*.odt)";
    QFileDialog* dialog = new QFileDialog(this);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    dialog->setWindowFlags(Qt::Sheet);
    dialog->setNameFilters(filters);
    dialog->setDirectory(QDir::home());
    dialog->open();

    connect(dialog, &QFileDialog::accepted, [=]() {
        dialog->deleteLater(); //This deletes the dialog once we return to the event loop.

        if (dialog->selectedNameFilter() == "theDocument HTML File (*.tdh)" || dialog->selectedNameFilter() == "Web Page (*.html)") {
            QFile file(dialog->selectedFiles().first());
            if (file.open(QFile::WriteOnly)) {
                file.write(currentDocument->toHtml().toUtf8());
                file.close();
                currentFile = dialog->selectedFiles().first();
                this->setWindowTitle("");
                this->setWindowFilePath(currentFile);
                currentDocument->setModified(false);
            } else {
                showMessageBox("Save Error", "Your document couldn't be saved.");
                return;
            }
        } else {
            if (showMessageBox("Save As OpenDocument Text?", "theDocument can save the file as an OpenDocument Text file, but you won't be able to edit it with theDocument.", QMessageBox::Save | QMessageBox::Cancel, QMessageBox::Save) == QMessageBox::Save) {
                QTextDocumentWriter writer(dialog->selectedFiles().first());
                writer.setFormat("ODF");
                if (!writer.write(currentDocument)) {
                    showMessageBox("Save Error", "Your document couldn't be saved.");
                    return;
                }
            } else {
                showMessageBox("Save Error", "Your document couldn't be saved.");
                return;
            }
        }


        if (closeOnceSaved) {
            this->close();
        }
    });
    connect(dialog, SIGNAL(rejected()), dialog, SLOT(deleteLater()));
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog* dialog = new QFileDialog(this);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setWindowFlags(Qt::Sheet);
    dialog->setNameFilter("theDocument HTML File (*.tdh)");
    dialog->open();

    connect(dialog, &QFileDialog::accepted, [=]() {
        if (dialog->selectedNameFilter() == "theDocument HTML File (*.tdh)") {
            QFile file(dialog->selectedFiles().first());
            file.open(QFile::ReadOnly);
            currentDocument->setHtml(QString(file.readAll()));
            file.close();

            currentFile = dialog->selectedFiles().first();
            this->setWindowTitle("");
            this->setWindowFilePath(currentFile);
        }
        currentDocument->setModified(false);

        dialog->deleteLater();
    });
    connect(dialog, SIGNAL(rejected()), dialog, SLOT(deleteLater()));
}

void MainWindow::on_actionNew_triggered()
{
    MainWindow* w = new MainWindow();
    w->show();
}

void MainWindow::on_actionRedo_triggered()
{
    currentDocument->redo();
}

void MainWindow::on_actionUndo_triggered()
{
    currentDocument->undo();
}

void MainWindow::on_documentView_cursorPositionChanged()
{
    this->currentCursor = ui->documentView->textCursor();
    on_documentView_currentCharFormatChanged(ui->documentView->currentCharFormat());
}

void MainWindow::on_documentView_currentCharFormatChanged(const QTextCharFormat &format)
{
    ui->fontFamily->setCurrentFont(ui->documentView->currentFont());
    ui->fontSize->setValue(ui->documentView->fontPointSize());

    if (ui->documentView->fontWeight() == QFont::Normal) {
        ui->actionBold->setChecked(false);
    } else {
        ui->actionBold->setChecked(true);
    }

    if (ui->documentView->fontItalic()) {
        ui->actionItalic->setChecked(true);
    } else {
        ui->actionItalic->setChecked(false);
    }

    if (ui->documentView->fontUnderline()) {
        ui->actionUnderline->setChecked(true);
    } else {
        ui->actionUnderline->setChecked(false);
    }

    if (format.fontStrikeOut()) {
        ui->actionStrikeout->setChecked(true);
    } else {
        ui->actionStrikeout->setChecked(false);
    }

}

void MainWindow::on_actionBold_triggered(bool arg1)
{
    if (arg1) {
        ui->documentView->setFontWeight(QFont::Bold);
    } else {
        ui->documentView->setFontWeight(QFont::Normal);
    }
}

void MainWindow::on_actionItalic_triggered(bool arg1)
{
    ui->documentView->setFontItalic(arg1);
}

void MainWindow::on_actionUnderline_triggered(bool arg1)
{
    ui->documentView->setFontUnderline(arg1);
}

void MainWindow::on_actionStrikeout_triggered(bool arg1)
{
    QTextCharFormat modifier = ui->documentView->currentCharFormat();
    modifier.setFontStrikeOut(arg1);
    ui->documentView->setCurrentCharFormat(modifier);
}

void MainWindow::on_actionPage_Break_triggered()
{
    currentCursor.insertHtml("<hr /><br />");
}

void MainWindow::on_actionInsert_Table_triggered()
{
    QTextTableFormat tableFormatting;
    tableFormatting.setBorderStyle(QTextTableFormat::BorderStyle_Solid);
    tableFormatting.setBorderBrush(QBrush(Qt::black));
    tableFormatting.setBorder(1);

    int col = QInputDialog::getInt(this, "Table", "Number of Columns");
    int row = QInputDialog::getInt(this, "Table", "Number of Rows");

    currentCursor.insertTable(row, col, tableFormatting);
}

void MainWindow::on_fontSize_valueChanged(double arg1)
{

    /*QTextCharFormat modifier = currentCursor.charFormat();

    QFont currentFont = modifier.font();
    currentFont.setPointSize(arg1);

    modifier.setFont(currentFont);
    currentCursor.setCharFormat(modifier);*/

    ui->documentView->setFontPointSize(arg1);
}

void MainWindow::on_fontFamily_currentFontChanged(const QFont &f)
{
    /*QTextCharFormat modifier = currentCursor.charFormat();
    modifier.setFontFamily(f.family());
    currentCursor.setCharFormat(modifier);*/

    ui->documentView->setCurrentFont(f);
}


void MainWindow::on_actionCopy_triggered()
{
    ui->documentView->copy();
}

void MainWindow::on_actionCut_triggered()
{
    ui->documentView->cut();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->documentView->paste();
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

bool MainWindow::allowClose() {
    return !hasHadEdits;
}
