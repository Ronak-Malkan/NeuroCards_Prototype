#include "deckListPanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>
#include <QLabel>  // Added missing include for QLabel

DeckListPanel::DeckListPanel(CardService* cardService, QWidget* parent)
    : QWidget(parent)
    , m_cardService(cardService)
    , m_listWidget(new QListWidget(this))
    , m_addDeckButton(new QPushButton(tr("Add Deck"), this))
    , m_deleteDeckButton(new QPushButton(tr("Delete"), this))
    , m_renameDeckButton(new QPushButton(tr("Rename"), this))
{
    // Set up list widget
    m_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    
    // Button layout
    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_addDeckButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_renameDeckButton);
    buttonLayout->addWidget(m_deleteDeckButton);
    
    // Main layout
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QLabel(tr("Available Decks:"), this));
    mainLayout->addWidget(m_listWidget);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
    
    // Connect signals
    connect(m_listWidget, &QListWidget::itemClicked,
            this, &DeckListPanel::onItemClicked);
    connect(m_listWidget, &QListWidget::customContextMenuRequested,
            this, &DeckListPanel::onListContextMenu);
    connect(m_addDeckButton, &QPushButton::clicked,
            this, &DeckListPanel::onAddDeckClicked);
    connect(m_deleteDeckButton, &QPushButton::clicked,
            this, &DeckListPanel::onDeleteDeckClicked);
    connect(m_renameDeckButton, &QPushButton::clicked,
            this, &DeckListPanel::onRenameDeckClicked);
    
    refreshList();
}

void DeckListPanel::refreshList() {
    m_listWidget->clear();
    auto deckNames = m_cardService->getDeckNames();
    for (const QString& name : deckNames) {
        m_listWidget->addItem(name);
    }
}

void DeckListPanel::onAddDeckClicked() {
    bool ok;
    QString name = QInputDialog::getText(
                    this, tr("Add Deck"),
                    tr("Deck name:"), QLineEdit::Normal,
                    "", &ok);
    if (ok && !name.isEmpty()) {
        if (m_cardService->createDeck(name)) {
            refreshList();
        } else {
            QMessageBox::warning(
                        this, tr("Error"),
                        tr("Failed to create deck. Maybe it already exists?"));
        }
    }
}

void DeckListPanel::onDeleteDeckClicked() {
    auto items = m_listWidget->selectedItems();
    if (items.isEmpty()) return;
    
    QString name = items.first()->text();
    if (QMessageBox::question(
                this, tr("Confirm Delete"),
                tr("Are you sure you want to delete the deck '%1'?")
                .arg(name)) == QMessageBox::Yes) {
        if (m_cardService->deleteDeck(name)) {
            refreshList();
        } else {
            QMessageBox::warning(
                        this, tr("Error"),
                        tr("Failed to delete deck."));
        }
    }
}

void DeckListPanel::onRenameDeckClicked() {
    auto items = m_listWidget->selectedItems();
    if (items.isEmpty()) return;
    
    QString oldName = items.first()->text();
    bool ok;
    QString newName = QInputDialog::getText(
                    this, tr("Rename Deck"),
                    tr("New name:"), QLineEdit::Normal,
                    oldName, &ok);
    if (ok && !newName.isEmpty() && newName != oldName) {
        if (m_cardService->renameDeck(oldName, newName)) {
            refreshList();
        } else {
            QMessageBox::warning(
                        this, tr("Error"),
                        tr("Failed to rename deck. Maybe the name is already used?"));
        }
    }
}

void DeckListPanel::onItemClicked(QListWidgetItem* item) {
    emit deckSelected(item->text());
}

void DeckListPanel::onListContextMenu(const QPoint& pos) {
    QListWidgetItem* item = m_listWidget->itemAt(pos);
    if (!item) return;
    
    QMenu menu(this);
    QAction* openAct = menu.addAction(tr("Open"));
    QAction* renameAct = menu.addAction(tr("Rename"));
    QAction* deleteAct = menu.addAction(tr("Delete"));
    QAction* sel = menu.exec(m_listWidget->mapToGlobal(pos));
    
    if (sel == openAct) {
        emit deckSelected(item->text());
    } else if (sel == renameAct) {
        m_listWidget->setCurrentItem(item);
        onRenameDeckClicked();
    } else if (sel == deleteAct) {
        m_listWidget->setCurrentItem(item);
        onDeleteDeckClicked();
    }
}
