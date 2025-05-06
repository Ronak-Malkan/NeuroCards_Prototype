#include "mainwindow.h"
#include "deckListPanel.h"
#include "deckDetailPanel.h"
#include "studyPanel.h"
#include "addCardDialog.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{
    // 1) Core model
    m_deckManager = new DeckManager(this);

    // 2) The stacked layout
    m_stack = new QStackedWidget(this);

    // 3) Instantiate each page
    m_deckListPanel   = new DeckListPanel(m_deckManager, this);
    m_deckDetailPanel = new DeckDetailPanel(m_deckManager, this);
    m_studyPanel      = new StudyPanel(m_deckManager, QString(), this);

    // 4) Add them in order
    m_stack->addWidget(m_deckListPanel);     // index 0
    m_stack->addWidget(m_deckDetailPanel);   // index 1
    m_stack->addWidget(m_studyPanel);        // index 2

    setCentralWidget(m_stack);
    resize(800, 600);
    setWindowTitle("NeuroCards");

    // 5) Connect navigation signals

    // DeckList → DeckDetail
    connect(m_deckListPanel, &DeckListPanel::deckSelected,
            this, [&](const QString& name){
        m_deckDetailPanel->setDeck(name);
        m_deckDetailPanel->refreshList();
        m_stack->setCurrentWidget(m_deckDetailPanel);
        setWindowTitle(tr("NeuroCards - %1").arg(name));
    });

    // DeckDetail → back to DeckList
    connect(m_deckDetailPanel, &DeckDetailPanel::backToDeckList,
            this, [=](){
        m_stack->setCurrentWidget(m_deckListPanel);
        setWindowTitle(tr("NeuroCards"));
    });

    // DeckDetail → StudyModePanel
    // Study Due only
    connect(m_deckDetailPanel, &DeckDetailPanel::studyDue,
            this, [&](const QString& name){
        m_studyPanel->setDueOnly(true);
        m_studyPanel->setDeck(name);
        m_stack->setCurrentWidget(m_studyPanel);
    });
    // Study Full deck
    connect(m_deckDetailPanel, &DeckDetailPanel::studyAll,
            this, [&](const QString& name){
        m_studyPanel->setDueOnly(false);
        m_studyPanel->setDeck(name);
        m_stack->setCurrentWidget(m_studyPanel);
    });
    // DeckDetail → AddCardDialog
    connect(m_deckDetailPanel, &DeckDetailPanel::addCardRequested,
            this, [&](const QString& name){
        AddCardDialog dlg(m_deckManager, name, -1, this);
        connect(&dlg, &AddCardDialog::cardAdded, this, [&](const QString& deck){
            m_deckDetailPanel->setDeck(deck);
            m_stack->setCurrentWidget(m_deckDetailPanel);
        });
        dlg.exec();
    });

    // StudyModePanel → back to DeckDetail
    connect(m_studyPanel, &StudyPanel::exitStudy,
            this, [=](){
        m_stack->setCurrentWidget(m_deckDetailPanel);
    });
}

MainWindow::~MainWindow() {}

