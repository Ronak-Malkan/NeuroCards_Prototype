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
    : QWidget(parent),
      m_isFrontVisible(true),
      m_deckManager(deckManager)
{
    // Set up stacked editors
    m_stack = new QStackedWidget(this);

    // Front editor
    m_frontEditor = new QTextEdit(this);
    m_frontEditor->setPlaceholderText("Front (question)");
    m_stack->addWidget(m_frontEditor);

    // Back editor
    m_backEditor = new QTextEdit(this);
    m_backEditor->setPlaceholderText("Back (answer)");
    m_stack->addWidget(m_backEditor);

    // Flip / Add buttons
    m_flipButton = new QPushButton("Flip", this);
    connect(m_flipButton, &QPushButton::clicked, this, &FlashcardEditorWidget::onFlipButtonClicked);

    m_addButton = new QPushButton("Add", this);
    connect(m_addButton, &QPushButton::clicked, this, &FlashcardEditorWidget::onAddButtonClicked);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_flipButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_addButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_stack);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
}

void FlashcardEditorWidget::onFlipButtonClicked() {
    // Toggle between front and back editors
    m_stack->setCurrentIndex(m_isFrontVisible ? 1 : 0);
    m_isFrontVisible = !m_isFrontVisible;
}

void FlashcardEditorWidget::onAddButtonClicked() {
    QString frontText = m_frontEditor->toPlainText().trimmed();
    QString backText  = m_backEditor->toPlainText().trimmed();

    if (frontText.isEmpty() || backText.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Both front and back text must be filled in.");
        return;
    }

    Flashcard newCard(frontText, backText);

    // NOTE: addFlashcardToDeck requires a deck name; using "Default" for now
    if (m_deckManager->addFlashcardToDeck("Default", newCard)) {
        QMessageBox::information(this, "Success", "Flashcard added successfully.");
        m_frontEditor->clear();
        m_backEditor->clear();
        m_stack->setCurrentIndex(0);
        m_isFrontVisible = true;
    } else {
        QMessageBox::critical(this, "Error", "Failed to add flashcard.");
    }
}

