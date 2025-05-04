#include "studyPanel.h"
#include <QRandomGenerator>
#include <random>
#include <QHBoxLayout>
#include <QMessageBox>

StudyPanel::StudyPanel(DeckManager* manager,
                       const QString& deckName,
                       QWidget* parent)
    : QWidget(parent),
      m_deckManager(manager),
      m_deckName(deckName),
      m_currentIndex(0),
      m_showingFront(true)
{
    setupUI();
    reloadDeck();
}

void StudyPanel::setDeck(const QString& deckName) {
    m_deckName = deckName;
    reloadDeck();
}

void StudyPanel::reloadDeck() {
    // Fetch & shuffle
    m_cards = m_deckManager->getFlashcards(m_deckName).toVector();
    std::mt19937 rng(QRandomGenerator::global()->generate());
    std::shuffle(m_cards.begin(), m_cards.end(), rng);
    m_currentIndex = 0;
    m_showingFront = true;
    loadCurrentCard();
}

void StudyPanel::setupUI() {
    m_layout = new QVBoxLayout(this);

    // Clickable card area
    m_cardLabel = new ClickableLabel(this);
    m_cardLabel->setAlignment(Qt::AlignCenter);
    m_cardLabel->setWordWrap(true);
    m_layout->addWidget(m_cardLabel);
    connect(m_cardLabel, &ClickableLabel::clicked, this, &StudyPanel::flipCard);

    // Prev / Next / Exit row
    auto* btnRow = new QHBoxLayout();
    m_prevButton = new QPushButton(tr("Prev"), this);
    connect(m_prevButton, &QPushButton::clicked, this, &StudyPanel::showPrevCard);
    btnRow->addWidget(m_prevButton);

    m_nextButton = new QPushButton(tr("Next"), this);
    connect(m_nextButton, &QPushButton::clicked, this, &StudyPanel::showNextCard);
    btnRow->addWidget(m_nextButton);

    btnRow->addStretch();

    m_exitButton = new QPushButton(tr("Exit"), this);
    connect(m_exitButton, &QPushButton::clicked, this, &StudyPanel::exitStudy);
    btnRow->addWidget(m_exitButton);

    m_layout->addLayout(btnRow);
    setLayout(m_layout);
}

void StudyPanel::loadCurrentCard() {
    if (m_cards.isEmpty()) {
        m_cardLabel->setText(tr("No cards to review."));
        m_prevButton->setEnabled(false);
        m_nextButton->setEnabled(false);
        return;
    }

    // Prepare front text
    const Flashcard& card = m_cards.at(m_currentIndex);
    m_showingFront = true;

    // For quiz cards, we’ll show “Question + options” on front
    if (card.isQuizCard()) {
        QString s = card.getFrontText() + "\n\n";
        auto opts = card.getOptions();
        for (int i = 0; i < opts.size(); ++i)
            s += QString("%1. %2\n").arg(i+1).arg(opts[i]);
        m_cardLabel->setText(s);

        // Cache the correct answer for flip
        m_correctAnswer = opts.at(card.getCorrectOptionIndex());
    } else {
        // Simple flip card
        m_cardLabel->setText(card.getFrontText());
        m_correctAnswer = card.getBackText();
    }

    // Prev/Next enablement
    m_prevButton->setEnabled(m_currentIndex > 0);
    m_nextButton->setEnabled(m_currentIndex < m_cards.size() - 1);
}

void StudyPanel::flipCard() {
    if (m_cards.isEmpty()) return;

    if (m_showingFront) {
        // Show back side
        m_cardLabel->setText(tr("→ ") + m_correctAnswer);
    } else {
        // Reload front
        loadCurrentCard();
        return;
    }
    m_showingFront = !m_showingFront;
}

void StudyPanel::showPrevCard() {
    if (m_currentIndex > 0) {
        m_currentIndex--;
        loadCurrentCard();
    }
}

void StudyPanel::showNextCard() {
    if (m_currentIndex + 1 < m_cards.size()) {
        m_currentIndex++;
        loadCurrentCard();
    }
}

void StudyPanel::onExitClicked() {
    emit exitStudy();
}

