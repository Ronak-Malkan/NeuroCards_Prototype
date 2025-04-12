#include "mainwindow.h"
#include "../core/deckmanager.h"
#include "flashCardEditorWidget.h"
#include "deckViewWidget.h"

#include <QSplitter>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Instantiate core object.
    m_deckManager = new DeckManager(this);

    // Instantiate UI widgets.
    m_editorWidget = new FlashcardEditorWidget(m_deckManager, this);
    m_deckViewWidget = new DeckViewWidget(m_deckManager, this);

    // Use a splitter for layout.
    m_splitter = new QSplitter(this);
    m_splitter->addWidget(m_editorWidget);
    m_splitter->addWidget(m_deckViewWidget);
    m_splitter->setStretchFactor(0, 2);
    m_splitter->setStretchFactor(1, 1);

    setCentralWidget(m_splitter);
    setWindowTitle("NeuroCards - Add Flashcards Prototype");
}

MainWindow::~MainWindow() {
    // Child widgets are deleted automatically.
}
