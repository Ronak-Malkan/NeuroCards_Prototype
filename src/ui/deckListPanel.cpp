#include "deckListPanel.h"
#include <QMenu>
#include <QMessageBox>
#include <QInputDialog>
#include <QHBoxLayout>
#include <QPushButton>

DeckListPanel::DeckListPanel(DeckManager* manager, QWidget* parent)
    : QWidget(parent), m_deckManager(manager)
{
    setupUI();
    connectSignals();
    refreshList();
}

void DeckListPanel::setupUI() {
    m_searchInput   = new QLineEdit(this);
    m_searchInput->setPlaceholderText(tr("Search decks..."));
    m_addDeckButton = new QPushButton(tr("+ Add Deck"), this);

    auto* topRow = new QHBoxLayout();
    topRow->addWidget(m_searchInput);
    topRow->addWidget(m_addDeckButton);

    // Deck list
    m_listWidget = new QListWidget(this);
    m_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Assemble
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topRow);
    mainLayout->addWidget(m_listWidget);
    setLayout(mainLayout);
}

void DeckListPanel::connectSignals() {
    connect(m_searchInput, &QLineEdit::textChanged,
            this, &DeckListPanel::onSearchTextChanged);
    connect(m_listWidget, &QListWidget::itemClicked,
            this, &DeckListPanel::onItemClicked);
    connect(m_listWidget, &QListWidget::customContextMenuRequested,
            this, &DeckListPanel::onListContextMenu);
    connect(m_addDeckButton, &QPushButton::clicked, [&]() {
        bool ok;
        QString name = QInputDialog::getText(this, tr("Create Deck"),
                                             tr("Deck name:"), QLineEdit::Normal,
                                             QString(), &ok);
        if (ok && !name.isEmpty()) {
            if (!m_deckManager->createDeck(name)) {
                QMessageBox::warning(this, tr("Error"),
                                     tr("Deck '%1' already exists.").arg(name));
            }
            refreshList();
        }
    });
}

void DeckListPanel::refreshList() {
    m_listWidget->clear();
    for (const QString& name : m_deckManager->getDeckNames()) {
        m_listWidget->addItem(name);
    }
}

void DeckListPanel::onSearchTextChanged(const QString& text) {
    for (int i = 0; i < m_listWidget->count(); ++i) {
        QListWidgetItem* item = m_listWidget->item(i);
        bool match = item->text().contains(text, Qt::CaseInsensitive);
        item->setHidden(!match);
    }
}

void DeckListPanel::onItemClicked(QListWidgetItem* item) {
    emit deckSelected(item->text());
}

void DeckListPanel::onListContextMenu(const QPoint &pos) {
    QListWidgetItem* item = m_listWidget->itemAt(pos);
    QMenu menu(this);
    QAction* createAct = menu.addAction(tr("New Deck"));
    QAction* renameAct = item ? menu.addAction(tr("Rename Deck")) : nullptr;
    QAction* deleteAct = item ? menu.addAction(tr("Delete Deck")) : nullptr;
    QAction* selected = menu.exec(m_listWidget->mapToGlobal(pos));

    if (selected == createAct) {
        bool ok;
        QString name = QInputDialog::getText(this, tr("Create Deck"),
                                           tr("Deck name:"), QLineEdit::Normal,
                                           QString(), &ok);
        if (ok && !name.isEmpty()) {
            if (!m_deckManager->createDeck(name)) {
                QMessageBox::warning(this, tr("Error"),
                                     tr("Cannot create deck '%1'.").arg(name));
            } else {
                refreshList();
            }
        }

    } else if (item && selected == renameAct) {
        QString oldName = item->text();
        bool ok;
        QString newName = QInputDialog::getText(this, tr("Rename Deck"),
                                               tr("New name:"), QLineEdit::Normal,
                                               oldName, &ok);
        if (ok && !newName.isEmpty()) {
            if (!m_deckManager->renameDeck(oldName, newName)) {
                QMessageBox::warning(this, tr("Error"),
                                     tr("Cannot rename deck '%1'.").arg(oldName));
            } else {
                refreshList();
            }
        }

    } else if (item && selected == deleteAct) {
        if (QMessageBox::question(this, tr("Confirm Delete"),
                                  tr("Delete deck '%1'?" ).arg(item->text()))
            == QMessageBox::Yes) {
            m_deckManager->deleteDeck(item->text());
            refreshList();
        }
    }
}
