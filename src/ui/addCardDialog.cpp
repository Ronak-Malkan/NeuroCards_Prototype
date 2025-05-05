#include "addCardDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

AddCardDialog::AddCardDialog(DeckManager* manager,
                             const QString& deckName,
                             int editIndex,
                             QWidget* parent)
    : QDialog(parent),
      m_deckManager(manager),
      m_initialDeck(deckName),
      m_editIndex(editIndex)
{
    setupUI();
    setWindowTitle(editIndex>=0
        ? tr("Edit Card in “%1”").arg(deckName)
        : tr("Add Card to “%1”").arg(deckName));
    
    if (m_editIndex >= 0) {
        const auto cards = m_deckManager->getFlashcards(deckName);
        const Flashcard& c = cards.at(m_editIndex);
        m_typeSelector->setCurrentText(c.isQuizCard() ? tr("Quiz") : tr("Flip"));
        onTypeChanged(m_typeSelector->currentText());
        if (c.isQuizCard()) {
            m_quizQuestionInput->setText(c.getFrontText());
            for (int i = 0; i < 4; ++i)
                m_quizOptionInputs[i]->setText(c.getOptions().at(i));
            m_quizCorrectSelector->setCurrentIndex(c.getCorrectOptionIndex());
        } else {
            m_flipQuestionInput->setText(c.getFrontText());
            m_flipAnswerInput->setText(c.getBackText());
        }
        m_addButton->setText(tr("Save"));
    }
    
    resize(400, 300);
}

void AddCardDialog::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);

    // Top controls: Type and Deck selectors
    auto* topLayout = new QHBoxLayout();
    m_typeSelector = new QComboBox(this);
    m_typeSelector->addItems({ tr("Flip"), tr("Quiz") });
    connect(m_typeSelector, &QComboBox::currentTextChanged,
            this, &AddCardDialog::onTypeChanged);

    m_deckSelector = new QComboBox(this);
    m_deckSelector->addItems(m_deckManager->getDeckNames());
    m_deckSelector->setCurrentText(m_initialDeck);

    topLayout->addWidget(new QLabel(tr("Type:"), this));
    topLayout->addWidget(m_typeSelector);
    topLayout->addStretch();
    topLayout->addWidget(new QLabel(tr("Deck:"), this));
    topLayout->addWidget(m_deckSelector);

    mainLayout->addLayout(topLayout);

    // Stacked pages for inputs
    m_stack = new QStackedWidget(this);

    // Flip page
    m_flipPage = new QWidget(this);
    {
        auto* flipLayout = new QVBoxLayout(m_flipPage);
        flipLayout->addWidget(new QLabel(tr("Question:"), this));
        m_flipQuestionInput = new QLineEdit(this);
        flipLayout->addWidget(m_flipQuestionInput);
        flipLayout->addWidget(new QLabel(tr("Answer:"), this));
        m_flipAnswerInput = new QLineEdit(this);
        flipLayout->addWidget(m_flipAnswerInput);
    }
    m_stack->addWidget(m_flipPage);

    // Quiz page
    m_quizPage = new QWidget(this);
    {
        auto* quizLayout = new QVBoxLayout(m_quizPage);
        quizLayout->addWidget(new QLabel(tr("Question:"), this));
        m_quizQuestionInput = new QLineEdit(this);
        quizLayout->addWidget(m_quizQuestionInput);
        for (int i = 0; i < 4; ++i) {
            m_quizOptionInputs[i] = new QLineEdit(this);
            m_quizOptionInputs[i]->setPlaceholderText(tr("Option %1").arg(i + 1));
            quizLayout->addWidget(m_quizOptionInputs[i]);
        }
        quizLayout->addWidget(new QLabel(tr("Correct Option:"), this));
        m_quizCorrectSelector = new QComboBox(this);
        m_quizCorrectSelector->addItems({ tr("Option 1"),
                                          tr("Option 2"),
                                          tr("Option 3"),
                                          tr("Option 4") });
        quizLayout->addWidget(m_quizCorrectSelector);
    }
    m_stack->addWidget(m_quizPage);

    mainLayout->addWidget(m_stack);

    // Buttons at bottom
    auto* buttonLayout = new QHBoxLayout();
    m_addButton = new QPushButton(tr("Add"), this);
    connect(m_addButton, &QPushButton::clicked, this, &AddCardDialog::onAddClicked);
    m_closeButton = new QPushButton(tr("Close"), this);
    connect(m_closeButton, &QPushButton::clicked, this, &AddCardDialog::onCloseClicked);

    buttonLayout->addStretch();
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_closeButton);
    mainLayout->addLayout(buttonLayout);

    // Initialize to Flip page
    m_stack->setCurrentIndex(0);
}

void AddCardDialog::onTypeChanged(const QString& type) {
    if (type == tr("Flip")) {
        m_stack->setCurrentWidget(m_flipPage);
    } else {
        m_stack->setCurrentWidget(m_quizPage);
    }
}

void AddCardDialog::onAddClicked() {
    const QString deck = m_deckSelector->currentText();
    Flashcard card("", "");

    if (m_typeSelector->currentText() == tr("Flip")) {
        QString q = m_flipQuestionInput->text().trimmed();
        QString a = m_flipAnswerInput->text().trimmed();
        if (q.isEmpty() || a.isEmpty()) {
            QMessageBox::warning(this, tr("Missing Data"),
                                 tr("Please fill out both question and answer."));
            return;
        }
        card = Flashcard(q, a);
    } else {
        QString q = m_quizQuestionInput->text().trimmed();
        if (q.isEmpty()) {
            QMessageBox::warning(this, tr("Missing Data"),
                                 tr("Please enter a question."));
            return;
        }
        QStringList opts;
        for (int i = 0; i < 4; ++i) {
            QString o = m_quizOptionInputs[i]->text().trimmed();
            if (o.isEmpty()) {
                QMessageBox::warning(this, tr("Missing Data"),
                                     tr("Please fill in all options."));
                return;
            }
            opts << o;
        }
        int idx = m_quizCorrectSelector->currentIndex();
        card = Flashcard(q, opts, idx);
    }

    bool ok;
    if (m_editIndex >= 0) {
        ok = m_deckManager->updateFlashcardInDeck(deck, m_editIndex, card);
        if (ok) emit cardEdited(deck, m_editIndex);
    } else {
        ok = m_deckManager->addFlashcardToDeck(deck, card);
        if (ok) emit cardAdded(deck);
    }

    if (!ok) {
        QMessageBox::critical(this, tr("Error"),
            m_editIndex >= 0 ? tr("Failed to save changes.") : tr("Failed to add card."));
        return;
    }
    accept();
}

void AddCardDialog::onCloseClicked() {
    accept();
}

void AddCardDialog::clearFlipInputs() {
    m_flipQuestionInput->clear();
    m_flipAnswerInput->clear();
}

void AddCardDialog::clearQuizInputs() {
    m_quizQuestionInput->clear();
    for (auto* w : m_quizOptionInputs) w->clear();
    m_quizCorrectSelector->setCurrentIndex(0);
}

