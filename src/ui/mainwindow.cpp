#include "mainwindow.h"
#include "deckListPanel.h"
#include "deckDetailPanel.h"
#include "studyModePanel.h"
#include "addCardDialog.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
    // Create DeckManager
    m_deckManager = new DeckManager(this);

    // Create the stacked widget
    m_stack = new QStackedWidget(this);

    // Instantiate each panel
    m_deckListPanel   = new DeckListPanel(m_deckManager, this);
    m_deckDetailPanel = new DeckDetailPanel(m_deckManager, this);
    m_studyPanel      = new StudyModePanel(m_deckManager, /* deckName will be set later */ QString(), this);

    // Add them to the stack in order
    m_stack->addWidget(m_deckListPanel);     // index 0
    m_stack->addWidget(m_deckDetailPanel);   // index 1
    m_stack->addWidget(m_studyPanel);        // index 2

    setCentralWidget(m_stack);
    resize(800, 600);
    setWindowTitle("NeuroCards");

    // Wire up the navigation signals
    connect(m_deckListPanel, &DeckListPanel::deckSelected,
            this, [&](const QString& name){
        // when user clicks a deck in the list
        m_deckDetailPanel->setDeck(name);
        m_stack->setCurrentWidget(m_deckDetailPanel);
    });

    connect(m_deckDetailPanel, &DeckDetailPanel::backToDeckList,
            this, [=](){
        // back arrow from detail → list
        m_stack->setCurrentWidget(m_deckListPanel);
    });

    connect(m_deckDetailPanel, &DeckDetailPanel::startStudy,
            this, [&](const QString& name){
        // user pressed Study on the detail page
        m_studyPanel->setDeck(name);
        m_stack->setCurrentWidget(m_studyPanel);
    });

    connect(m_deckDetailPanel, &DeckDetailPanel::addCardRequested,
            this, [&](const QString& name){
        AddCardDialog dlg(m_deckManager, name, this);
        connect(&dlg, &AddCardDialog::cardAdded, this, [&](const QString& deck){
            m_deckDetailPanel->setDeck(deck);
            m_stack->setCurrentWidget(m_deckDetailPanel);
        });
        dlg.exec();
    });


    connect(m_studyPanel, &StudyModePanel::exitStudy,
            this, [=](){
        // exit back to the deck detail page
        m_stack->setCurrentWidget(m_deckDetailPanel);
    });

}

MainWindow::~MainWindow() {}

