#include "studyPanel.h"
#include "../core/quizcard.h"
#include <QRandomGenerator>
#include <algorithm>
#include <QDebug>

// Quality grading definitions
static struct GradeInfo { const char* label; int quality; } gradeInfo[4] = {
    { "Again", 1 },
    { "Hard",  3 },
    { "Good",  4 },
    { "Easy",  5 }
};

StudyPanel::StudyPanel(CardService* cardService,
                       const QString& deckName,
                       QWidget* parent)
    : QWidget(parent),
      m_cardService(cardService),
      m_deckName(deckName),
      m_orderIndices(),
      m_currentIndex(0),
      m_showingFront(true),
      m_dueOnly(false)
{
    setMinimumSize(600, 400);
    setMaximumSize(800, 600);
    setupUI();
    reloadDeck();
}

void StudyPanel::setDeck(const QString& deckName) {
    m_deckName = deckName;
    reloadDeck();
}

void StudyPanel::reloadDeck() {
    // 1) Grab the full deck once
    QVector<Flashcard*> allCards = m_cardService->getFlashcards(m_deckName);

    // 2) Build a filtered+shuffled list of indices into that deck
    m_orderIndices.clear();
    QDate today = QDate::currentDate();
    for (int i = 0; i < allCards.size(); ++i) {
        if (!m_dueOnly || allCards[i]->getNextReview() <= today) {
            m_orderIndices.append(i);
        }
    }
    std::shuffle(
      m_orderIndices.begin(),
      m_orderIndices.end(),
      std::mt19937{ (uint)QRandomGenerator::global()->generate() }
    );

    // 3) Reset position & state
    m_currentIndex = 0;
    m_showingFront = true;

    // 4) Show the first card (or "no cards")
    loadCurrentCard();
}

void StudyPanel::setDueOnly(bool dueOnly) {
    m_dueOnly = dueOnly;
}

void StudyPanel::setupUI() {
    m_layout = new QVBoxLayout(this);

    // Card area
    m_cardLabel = new ClickableLabel(this);
    m_cardLabel->setAlignment(Qt::AlignCenter);
    m_cardLabel->setWordWrap(true);
    QFont f = m_cardLabel->font();
    f.setPointSize(24);
    m_cardLabel->setFont(f);
    m_cardLabel->setMinimumHeight(200);
    connect(m_cardLabel, &ClickableLabel::clicked, this, &StudyPanel::flipCard);
    m_layout->addWidget(m_cardLabel);

    // Grading buttons row
    auto* gradeLayout = new QHBoxLayout();
    gradeLayout->addStretch();
    for (int i = 0; i < 4; ++i) {
        auto& g = gradeInfo[i];
        auto* btn = new QPushButton(tr(g.label), this);
        btn->setProperty("quality", g.quality);
        btn->setFixedSize(80, 40);
        connect(btn, &QPushButton::clicked, this, &StudyPanel::onGradeClicked);
        gradeLayout->addWidget(btn);
        m_gradeButtons[i] = btn;
    }
    gradeLayout->addStretch();
    m_layout->addLayout(gradeLayout);

    // Navigation / Exit row
    m_navLayout = new QHBoxLayout();
    m_prevButton = new QPushButton(tr("Prev"), this);
    m_nextButton = new QPushButton(tr("Next"), this);
    m_exitButton = new QPushButton(tr("Exit"), this);
    m_prevButton->setFixedSize(80, 40);
    m_nextButton->setFixedSize(80, 40);
    m_exitButton->setFixedSize(80, 40);
    connect(m_prevButton, &QPushButton::clicked, this, &StudyPanel::showPrevCard);
    connect(m_nextButton, &QPushButton::clicked, this, &StudyPanel::showNextCard);
    connect(m_exitButton, &QPushButton::clicked, this, &StudyPanel::studyExit);
    m_navLayout->addWidget(m_prevButton);
    m_navLayout->addWidget(m_nextButton);
    m_navLayout->addStretch();
    m_navLayout->addWidget(m_exitButton);
    m_layout->addLayout(m_navLayout);

    setLayout(m_layout);
}

void StudyPanel::loadCurrentCard() {
    // No cards at all (after filtering)?
    if (m_orderIndices.isEmpty()) {
        m_cardLabel->setText(tr("No cards to review."));
        m_prevButton->setEnabled(false);
        m_nextButton->setEnabled(false);
        for (auto* btn : m_gradeButtons) btn->setEnabled(false);
        return;
    }

    // Fetch full deck and map our shuffled index into it
    QVector<Flashcard*> allCards = m_cardService->getFlashcards(m_deckName);
    int origIdx = m_orderIndices[m_currentIndex];
    Flashcard* card = allCards.at(origIdx);

    m_showingFront = true;

    // Show the "front" or question/options
    if (card->isQuizCard()) {
        // Cast to QuizCard to access quiz-specific methods
        QuizCard* quizCard = dynamic_cast<QuizCard*>(card);
        if (quizCard) {
            QString txt = quizCard->getFrontText() + "\n\n";
            auto options = quizCard->getOptions();
            for (int i = 0; i < options.size(); ++i)
                txt += QString("%1. %2\n")
                         .arg(i+1)
                         .arg(options[i]);
            m_cardLabel->setText(txt);
        }
    } else {
        m_cardLabel->setText(card->getFrontText());
    }

    // Disable grading until flip
    for (auto* btn : m_gradeButtons) btn->setEnabled(false);

    // Prev/Next buttons only when in‐range
    m_prevButton->setEnabled(m_currentIndex > 0);
    m_nextButton->setEnabled(m_currentIndex + 1 < m_orderIndices.size());
}

void StudyPanel::flipCard() {
    if (m_orderIndices.isEmpty()) return;

    QVector<Flashcard*> allCards = m_cardService->getFlashcards(m_deckName);
    int origIdx = m_orderIndices[m_currentIndex];
    Flashcard* card = allCards.at(origIdx);

    if (m_showingFront) {
        // show back or correct answer
        if (card->isQuizCard()) {
            // Cast to QuizCard to access quiz-specific methods
            QuizCard* quizCard = dynamic_cast<QuizCard*>(card);
            if (quizCard) {
                auto options = quizCard->getOptions();
                int correctIndex = quizCard->getCorrectIndex();
                if (correctIndex >= 0 && correctIndex < options.size()) {
                    m_cardLabel->setText(QStringLiteral("→ ") + options.at(correctIndex));
                }
            }
        } else {
            m_cardLabel->setText(QStringLiteral("→ ") + card->getBackText());
        }

        for (auto* btn : m_gradeButtons)
            btn->setEnabled(true);
    } else {
        loadCurrentCard();
        return;
    }
    m_showingFront = !m_showingFront;
}

void StudyPanel::onGradeClicked() {
    auto* btn = qobject_cast<QPushButton*>(sender());
    int quality = btn->property("quality").toInt();

    // record on the real deck entry
    int origIdx = m_orderIndices[m_currentIndex];
    m_cardService->recordCardResult(m_deckName, origIdx, quality);

    // move on or exit
    if (m_currentIndex + 1 >= m_orderIndices.size())
        studyExit();
    else
        showNextCard();
}


void StudyPanel::showPrevCard() {
    if (m_currentIndex > 0) {
        --m_currentIndex;
        loadCurrentCard();
    }
}

void StudyPanel::showNextCard() {
    if (m_currentIndex + 1 < m_orderIndices.size()) {
        ++m_currentIndex;
        loadCurrentCard();
    }
}

void StudyPanel::studyExit() {
    emit exitStudy();
}
