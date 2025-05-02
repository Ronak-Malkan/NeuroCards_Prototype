#include "mainwindow.h"
#include "../core/deckmanager.h"
// #include "flashCardEditorWidget.h"
#include "deckViewWidget.h"
#include "studyModePanel.h" 

#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Initialize backend
    m_deckManager = new DeckManager(this);

    // Initialize widgets
//    m_editorWidget = new FlashcardEditorWidget(m_deckManager, this);
    m_deckViewWidget = new DeckViewWidget(m_deckManager, this);
    m_studyPanel = new StudyModePanel(m_deckManager, this);
    connect(m_studyPanel, &StudyModePanel::cardAdded, m_deckViewWidget, &DeckViewWidget::refreshList);

    // Layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

//    mainLayout->addWidget(m_editorWidget);
    mainLayout->addWidget(m_deckViewWidget);
    mainLayout->addWidget(m_studyPanel);  

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setWindowTitle("NeuroCards");
}

MainWindow::~MainWindow() {
    // No manual deletion needed (Qt handles children)
}

