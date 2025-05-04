#include "deckDetailPanel.h"
#include <QMessageBox>

DeckDetailPanel::DeckDetailPanel(DeckManager* manager, QWidget* parent)
    : QWidget(parent), m_deckManager(manager)
{
    setupUI();
    connectSignals();
}

void DeckDetailPanel::setupUI() {
    // Top bar: Back button + Title
    m_backButton = new QPushButton(tr("← Back"), this);
    m_titleLabel = new QLabel(tr("Deck: (none)"), this);
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(m_backButton);
    topLayout->addWidget(m_titleLabel);
    topLayout->addStretch();

    // Stats bar: count, study, add
    m_countLabel = new QLabel(tr("Cards: 0"), this);
    m_studyButton = new QPushButton(tr("Study"), this);
    m_addCardButton = new QPushButton(tr("Add Card"), this);
    QHBoxLayout* statsLayout = new QHBoxLayout();
    statsLayout->addWidget(m_countLabel);
    statsLayout->addStretch();
    statsLayout->addWidget(m_studyButton);
    statsLayout->addWidget(m_addCardButton);

    // Card list
    m_listWidget = new QListWidget(this);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(statsLayout);
    mainLayout->addWidget(m_listWidget);
    setLayout(mainLayout);
}

void DeckDetailPanel::connectSignals() {
    connect(m_backButton, &QPushButton::clicked, this, &DeckDetailPanel::onBackClicked);
    connect(m_studyButton, &QPushButton::clicked, this, &DeckDetailPanel::onStudyClicked);
    connect(m_addCardButton, &QPushButton::clicked, this, &DeckDetailPanel::onAddCardClicked);
    connect(m_listWidget, &QListWidget::itemClicked, this, &DeckDetailPanel::onItemClicked);
}

void DeckDetailPanel::setDeck(const QString& deckName) {
    m_deckName = deckName;
    m_titleLabel->setText(tr("Deck: %1").arg(deckName));
    refreshList();
}

void DeckDetailPanel::refreshList() {
    m_listWidget->clear();
    auto cards = m_deckManager->getFlashcards(m_deckName);
    m_countLabel->setText(tr("Cards: %1").arg(cards.size()));
    for (const Flashcard& card: cards) {
        m_listWidget->addItem(card.getFrontText());
    }
}

void DeckDetailPanel::onBackClicked() {
    emit backToDeckList();
}

void DeckDetailPanel::onStudyClicked() {
    emit startStudy(m_deckName);
}

void DeckDetailPanel::onAddCardClicked() {
    emit addCardRequested(m_deckName);
}

void DeckDetailPanel::onItemClicked(QListWidgetItem* item) {
    int row = m_listWidget->row(item);
    auto cards = m_deckManager->getFlashcards(m_deckName);
    if (row >= 0 && row < cards.size()) {
        CardPreviewDialog dlg(cards.at(row), this);
        dlg.exec();
    }
}

