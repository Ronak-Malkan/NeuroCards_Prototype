#include "mainwindow.h"
#include "deckListPanel.h"
#include "deckDetailPanel.h"
#include "flashCardEditorWidget.h"
#include "studyPanel.h"
#include "addCardDialog.h"
#include "../core/deckmanager.h"
#include "../core/cardserviceimpl.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_stackedWidget(new QStackedWidget(this)),
      m_deckManager(new DeckManager(this)),
      m_cardService(new CardServiceImpl(m_deckManager))
{
    resize(800, 600);
    setWindowTitle(tr("NeuroCards"));

    // Create widget stack
    setCentralWidget(m_stackedWidget);

    // Set up deck list panel
    m_deckListPanel = new DeckListPanel(m_cardService, this);
    m_stackedWidget->addWidget(m_deckListPanel);

    // Set up deck detail panel
    m_deckDetailPanel = new DeckDetailPanel(m_cardService, this);
    m_stackedWidget->addWidget(m_deckDetailPanel);

    // Set up card editor widget
    m_cardEditorWidget = new FlashcardEditorWidget(m_cardService, this);
    m_stackedWidget->addWidget(m_cardEditorWidget);

    // Set up study panel
    m_studyPanel = new StudyPanel(m_cardService, "Default", this);
    m_stackedWidget->addWidget(m_studyPanel);

    // Start with the deck list
    m_stackedWidget->setCurrentWidget(m_deckListPanel);

    // Connect signals
    connect(m_deckListPanel, &DeckListPanel::deckSelected,
            this, &MainWindow::onDeckSelected);
    connect(m_deckDetailPanel, &DeckDetailPanel::backToDeckList,
            this, &MainWindow::onBackToDeckList);
    connect(m_deckDetailPanel, &DeckDetailPanel::addCardRequested,
            this, &MainWindow::onAddCardRequested);
    connect(m_deckDetailPanel, &DeckDetailPanel::studyDue,
            this, &MainWindow::onStudyDue);
    connect(m_deckDetailPanel, &DeckDetailPanel::studyAll,
            this, &MainWindow::onStudyAll);
    connect(m_studyPanel, &StudyPanel::exitStudy,
            this, &MainWindow::onExitStudy);
}

MainWindow::~MainWindow() {
    delete m_cardService; // Make sure to delete CardService before DeckManager
}

void MainWindow::onDeckSelected(const QString &deckName) {
    m_deckDetailPanel->setDeck(deckName);
    m_stackedWidget->setCurrentWidget(m_deckDetailPanel);
}

void MainWindow::onBackToDeckList() {
    m_stackedWidget->setCurrentWidget(m_deckListPanel);
}

void MainWindow::onAddCardRequested(const QString &deckName) {
    AddCardDialog dlg(m_cardService, deckName, -1, this);
    if (dlg.exec() == QDialog::Accepted) {
        m_deckDetailPanel->refreshList();
    }
}

void MainWindow::onStudyDue(const QString &deckName) {
    m_studyPanel->setDeck(deckName);
    m_studyPanel->setDueOnly(true);
    m_studyPanel->reloadDeck();
    m_stackedWidget->setCurrentWidget(m_studyPanel);
}

void MainWindow::onStudyAll(const QString &deckName) {
    m_studyPanel->setDeck(deckName);
    m_studyPanel->setDueOnly(false);
    m_studyPanel->reloadDeck();
    m_stackedWidget->setCurrentWidget(m_studyPanel);
}

void MainWindow::onExitStudy() {
    // Return to whichever screen we came from
    m_stackedWidget->setCurrentWidget(m_deckDetailPanel);
}

