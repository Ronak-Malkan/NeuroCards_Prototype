#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSplitter;
class DeckManager;
class FlashcardEditorWidget;
class DeckViewWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QSplitter *m_splitter;
    DeckManager *m_deckManager;
    FlashcardEditorWidget *m_editorWidget;
    DeckViewWidget *m_deckViewWidget;
};

#endif // MAINWINDOW_H
