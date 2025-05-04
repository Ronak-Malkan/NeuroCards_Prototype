#include "deckListPanel.h"
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>

DeckListPanel::DeckListPanel(DeckManager* manager, QWidget* parent)
    : QWidget(parent), m_deckManager(manager)
{
    m_listWidget = new QListWidget(this);
    m_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_listWidget, &QListWidget::itemClicked, this, &DeckListPanel::onItemClicked);
    connect(m_listWidget, &QListWidget::customContextMenuRequested,
            this, &DeckListPanel::onContextMenuRequested);

    m_addButton = new QPushButton("+ Add Deck", this);
    connect(m_addButton, &QPushButton::clicked, this, &DeckListPanel::onAddDeck);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_listWidget);
    layout->addWidget(m_addButton);
    setLayout(layout);

    refreshList();
}

void DeckListPanel::refreshList() {
    m_listWidget->clear();
    for (const QString& name : m_deckManager->getDeckNames())
        m_listWidget->addItem(name);
}

void DeckListPanel::onAddDeck() {
    bool ok;
    QString name = QInputDialog::getText(this, "New Deck", "Deck name:", QLineEdit::Normal, "", &ok);
    if (ok && !name.isEmpty()) {
        if (!m_deckManager->createDeck(name))
            QMessageBox::warning(this, "Error", "Could not create deck (duplicate or empty)");
        refreshList();
    }
}

void DeckListPanel::onItemClicked(QListWidgetItem* item) {
    emit deckSelected(item->text());
}

void DeckListPanel::onContextMenuRequested(const QPoint& pos) {
    auto* item = m_listWidget->itemAt(pos);
    if (!item) return;

    QMenu menu(this);
    menu.addAction("Rename", this, &DeckListPanel::renameDeck);
    menu.addAction("Delete", this, &DeckListPanel::deleteDeck);
    // store the target row in the widget’s property
    m_listWidget->setProperty("contextRow", m_listWidget->row(item));
    menu.exec(m_listWidget->mapToGlobal(pos));
}

void DeckListPanel::renameDeck() {
    int row = m_listWidget->property("contextRow").toInt();
    QString oldName = m_listWidget->item(row)->text();
    bool ok;
    QString newName = QInputDialog::getText(this, "Rename Deck",
                                            "New name:", QLineEdit::Normal, oldName, &ok);
    if (ok && !newName.isEmpty()) {
        if (!m_deckManager->renameDeck(oldName, newName))
            QMessageBox::warning(this, "Error", "Rename failed (duplicate or empty)");
        refreshList();
    }
}

void DeckListPanel::deleteDeck() {
    int row = m_listWidget->property("contextRow").toInt();
    QString name = m_listWidget->item(row)->text();
    if (QMessageBox::question(this, "Delete Deck",
          QString("Delete '%1'?").arg(name))
        == QMessageBox::Yes) {
        m_deckManager->deleteDeck(name);
        refreshList();
    }
}

