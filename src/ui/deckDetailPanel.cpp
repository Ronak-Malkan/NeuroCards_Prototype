#include "deckDetailPanel.h"
#include "statsDialog.h"
#include "addCardDialog.h"
#include "cardPreviewDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QMenu>

DeckDetailPanel::DeckDetailPanel(DeckManager* manager, QWidget* parent)
    : QWidget(parent)
    , m_deckManager(manager)
    , m_deckName          ()
    , m_backButton        (new QPushButton(tr("← Back"), this))
    , m_titleLabel        (new QLabel(this))
    , m_countLabel        (new QLabel(this))
    , m_statsButton       (new QPushButton(tr("Stats"), this))
    , m_studyAllButton    (new QPushButton(tr("Study All"), this))
    , m_studyDueButton    (new QPushButton(tr("Study Due"), this))
    , m_addCardButton     (new QPushButton(tr("Add Card"), this))
    , m_listWidget        (new QListWidget(this))
{
    // Top‐bar layout
    auto* topLayout = new QHBoxLayout();
    topLayout->addWidget(m_backButton);
    topLayout->addWidget(m_titleLabel);
    m_titleLabel->setText(tr("Deck: (none)"));
    m_titleLabel->setStyleSheet("font-weight: bold; margin-left:8px");
    topLayout->addStretch();
    topLayout->addWidget(m_countLabel);
    topLayout->addWidget(m_statsButton);
    topLayout->addWidget(m_studyDueButton);
    topLayout->addWidget(m_studyAllButton);
    topLayout->addWidget(m_addCardButton);

    // List of cards
    m_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Main layout
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_listWidget);
    setLayout(mainLayout);

    // Signal wiring
    connect(m_backButton,     &QPushButton::clicked,  
            this, &DeckDetailPanel::onBackClicked);
    connect(m_statsButton,    &QPushButton::clicked,  
            this, &DeckDetailPanel::onStatsClicked);
    connect(m_addCardButton,  &QPushButton::clicked,  
            this, &DeckDetailPanel::onAddCardClicked);
    connect(m_studyDueButton, &QPushButton::clicked,  
            this, [this]{ emit studyDue(m_deckName); });
    connect(m_studyAllButton, &QPushButton::clicked,  
            this, [this]{ emit studyAll(m_deckName); });
    connect(m_listWidget,     &QListWidget::itemClicked,  
            this, &DeckDetailPanel::onItemClicked);
    connect(m_listWidget,     &QListWidget::customContextMenuRequested,      
            this, &DeckDetailPanel::onListContextMenu);
}

void DeckDetailPanel::setDeck(const QString& deckName) {
    m_deckName = deckName;
    m_titleLabel->setText(tr("Deck: %1").arg(deckName));
    refreshList();
}

void DeckDetailPanel::refreshList() {
    auto cards = m_deckManager->getFlashcards(m_deckName);
    m_listWidget->clear();
    for (const Flashcard& c : cards)
        m_listWidget->addItem(c.getFrontText());
    m_countLabel->setText(tr("Cards: %1").arg(cards.size()));
}

void DeckDetailPanel::onStatsClicked() {
    StatsDialog dlg(m_deckManager, m_deckName, this);
    dlg.exec();
}

void DeckDetailPanel::onAddCardClicked() {
    emit addCardRequested(m_deckName);
}

void DeckDetailPanel::onItemClicked(QListWidgetItem* item) {
    int row = m_listWidget->row(item);
    CardPreviewDialog dlg(m_deckManager, m_deckName, row, this);
    dlg.exec();
}

void DeckDetailPanel::onListContextMenu(const QPoint& pos) {
    QListWidgetItem* item = m_listWidget->itemAt(pos);
    if (!item) return;

    int row = m_listWidget->row(item);
    // Fetch full card list to validate
    auto cards = m_deckManager->getFlashcards(m_deckName);
    if (row < 0 || row >= cards.size()) return;

    QMenu menu(this);
    QAction* editAct   = menu.addAction(tr("Edit Card"));
    QAction* deleteAct = menu.addAction(tr("Delete Card"));
    QAction* sel       = menu.exec(m_listWidget->mapToGlobal(pos));

    if (sel == deleteAct) {
        if (QMessageBox::question(this, tr("Delete Card"),
                                  tr("Are you sure you want to delete this card?")) 
            == QMessageBox::Yes) {
            m_deckManager->removeFlashcardFromDeck(m_deckName, row);
            refreshList();
        }
    }
    else if (sel == editAct) {
        // This guard ensures we never pass a bad index
        AddCardDialog dlg(m_deckManager, m_deckName, row, this);
        if (dlg.exec() == QDialog::Accepted) {
            refreshList();
        }
    }
}


void DeckDetailPanel::onBackClicked() {
    emit backToDeckList();
}

