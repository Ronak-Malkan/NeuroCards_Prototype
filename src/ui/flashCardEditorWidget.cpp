#include "flashCardEditorWidget.h"
#include "../core/deckmanager.h"
#include "../core/flashcard.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>

FlashcardEditorWidget::FlashcardEditorWidget(DeckManager *deckManager, QWidget *parent)
    : QWidget(parent), m_isFrontVisible(true), m_deckManager(deckManager)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    m_stack = new QStackedWidget(this);
    m_frontEditor = new QTextEdit(this);
    m_frontEditor->setPlaceholderText("Enter front side text...");
    m_backEditor = new QTextEdit(this);
    m_backEditor->setPlaceholderText("Enter back side text...");
    m_stack->addWidget(m_frontEditor);
    m_stack->addWidget(m_backEditor);
    m_stack->setCurrentIndex(0);

    mainLayout->addWidget(m_stack);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_flipButton = new QPushButton("Flip", this);
    m_addButton = new QPushButton("Add", this);

    buttonLayout->addWidget(m_flipButton);
    buttonLayout->addWidget(m_addButton);
    mainLayout->addLayout(buttonLayout);

    connect(m_flipButton, &QPushButton::clicked, this, &FlashcardEditorWidget::onFlipButtonClicked);
    connect(m_addButton, &QPushButton::clicked, this, &FlashcardEditorWidget::onAddButtonClicked);
}

void FlashcardEditorWidget::onFlipButtonClicked() {
    m_stack->setCurrentIndex(m_isFrontVisible ? 1 : 0);
    m_isFrontVisible = !m_isFrontVisible;
}

void FlashcardEditorWidget::onAddButtonClicked() {
    QString frontText = m_frontEditor->toPlainText().trimmed();
    QString backText = m_backEditor->toPlainText().trimmed();

    if (frontText.isEmpty() || backText.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Both front and back text must be filled in.");
        return;
    }

    Flashcard newCard(frontText, backText);
    if (m_deckManager->addFlashcard(newCard)) {
        QMessageBox::information(this, "Success", "Flashcard added successfully.");
        m_frontEditor->clear();
        m_backEditor->clear();
        m_stack->setCurrentIndex(0);
        m_isFrontVisible = true;
    } else {
        QMessageBox::critical(this, "Error", "Failed to add flashcard.");
    }
}
