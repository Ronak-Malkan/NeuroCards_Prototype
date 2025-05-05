#include "studyPanel.h"
#include <QRandomGenerator>
#include <algorithm>

StudyPanel::StudyPanel(DeckManager* manager,
                       const QString& deckName,
                       QWidget* parent)
    : QWidget(parent),
      m_deckManager(manager),
      m_deckName(deckName),
      m_currentIndex(0),
      m_showingFront(true)
{
    // Clamp size
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
    // Fetch and store cards
    m_cards = m_deckManager->getFlashcards(m_deckName).toVector();
    // Build shuffled index map
    m_orderIndices.clear();
    int n = m_cards.size();
    for (int i = 0; i < n; ++i)
        m_orderIndices.append(i);
    std::shuffle(m_orderIndices.begin(), m_orderIndices.end(),
                 std::mt19937{ (uint)QRandomGenerator::global()->generate() });
    m_currentIndex = 0;
    m_showingFront = true;
    loadCurrentCard();
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

    // Response buttons
    m_responseLayout = new QHBoxLayout();
    m_correctButton = new QPushButton(tr("Correct"), this);
    m_wrongButton   = new QPushButton(tr("Wrong"), this);
    m_correctButton->setFixedSize(100, 40);
    m_wrongButton->setFixedSize(100, 40);
    connect(m_correctButton, &QPushButton::clicked, this, &StudyPanel::markCorrect);
    connect(m_wrongButton,   &QPushButton::clicked, this, &StudyPanel::markWrong);
    m_responseLayout->addStretch();
    m_responseLayout->addWidget(m_correctButton);
    m_responseLayout->addWidget(m_wrongButton);
    m_responseLayout->addStretch();
    m_layout->addLayout(m_responseLayout);

    // Navigation / Exit
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
    if (m_cards.isEmpty()) {
        m_cardLabel->setText(tr("No cards to review."));
        m_prevButton->setEnabled(false);
        m_nextButton->setEnabled(false);
        m_correctButton->setEnabled(false);
        m_wrongButton->setEnabled(false);
        return;
    }

    int origIdx = m_orderIndices[m_currentIndex];
    const Flashcard& card = m_cards.at(origIdx);
    m_showingFront = true;

    // Show front side
    if (card.isQuizCard()) {
        QString text = card.getFrontText() + "\n\n";
        auto opts = card.getOptions();
        for (int i = 0; i < opts.size(); ++i)
            text += QString("%1. %2\n").arg(i+1).arg(opts[i]);
        m_cardLabel->setText(text);
    } else {
        m_cardLabel->setText(card.getFrontText());
    }

    // Disable responses until flipped
    m_correctButton->setEnabled(false);
    m_wrongButton->setEnabled(false);

    // Prev/Next enablement
    m_prevButton->setEnabled(m_currentIndex > 0);
    m_nextButton->setEnabled(m_currentIndex < m_orderIndices.size() - 1);
}

void StudyPanel::flipCard() {
    if (m_cards.isEmpty()) return;
    int origIdx = m_orderIndices[m_currentIndex];
    const Flashcard& card = m_cards.at(origIdx);

    if (m_showingFront) {
        // Show back or correct answer
        if (card.isQuizCard()) {
            m_cardLabel->setText(tr("→ ") + card.getOptions().at(card.getCorrectOptionIndex()));
        } else {
            m_cardLabel->setText(card.getBackText());
        }
        // Enable responses for all card types
        m_correctButton->setEnabled(true);
        m_wrongButton->setEnabled(true);
    } else {
        loadCurrentCard();
        return;
    }
    m_showingFront = !m_showingFront;
}

void StudyPanel::markCorrect() {
    int origIdx = m_orderIndices[m_currentIndex];
    m_deckManager->recordCardResult(m_deckName, origIdx, true);
    if (m_currentIndex + 1 >= m_orderIndices.size()) {
        studyExit();
    } else {
        showNextCard();
    }
}

void StudyPanel::markWrong() {
    int origIdx = m_orderIndices[m_currentIndex];
    m_deckManager->recordCardResult(m_deckName, origIdx, false);
    if (m_currentIndex + 1 >= m_orderIndices.size()) {
        studyExit();
    } else {
        showNextCard();
    }
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
