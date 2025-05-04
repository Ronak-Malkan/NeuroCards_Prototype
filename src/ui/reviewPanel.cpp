#include "reviewPanel.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include <random>

ReviewPanel::ReviewPanel(DeckManager* manager,
                         const QString& deckName,
                         QWidget *parent)
    : QWidget(parent),
      deckManager_(manager),
      deckName_(deckName),
      currentIndex_(0)
{
    // Initial load
    cards_ = deckManager_->getFlashcards(deckName_).toVector();
    std::mt19937 rng(QRandomGenerator::global()->generate());
    std::shuffle(cards_.begin(), cards_.end(), rng);

    setupUI();
    loadCurrentCard();
}

void ReviewPanel::reloadDeck() {
    cards_ = deckManager_->getFlashcards(deckName_).toVector();
    std::mt19937 rng(QRandomGenerator::global()->generate());
    std::shuffle(cards_.begin(), cards_.end(), rng);
    currentIndex_ = 0;
    loadCurrentCard();
}

void ReviewPanel::setupUI() {
    layout_ = new QVBoxLayout(this);

    // Card text label
    cardLabel_ = new QLabel(this);
    cardLabel_->setAlignment(Qt::AlignCenter);
    cardLabel_->setWordWrap(true);
    layout_->addWidget(cardLabel_);

    // Exit Review Mode button
    exitButton_ = new QPushButton(tr("Exit Review Mode"), this);
    connect(exitButton_, &QPushButton::clicked, this, &ReviewPanel::exitReview);
    layout_->addWidget(exitButton_);

    // Quiz option buttons (hidden initially)
    for (int i = 0; i < 4; ++i) {
        QPushButton* btn = new QPushButton(this);
        btn->hide();
        connect(btn, &QPushButton::clicked, this, &ReviewPanel::checkQuizAnswer);
        optionButtons_.append(btn);
        layout_->addWidget(btn);
    }

    // Next Card button
    nextButton_ = new QPushButton(tr("Next Card"), this);
    connect(nextButton_, &QPushButton::clicked, this, &ReviewPanel::showNextCard);
    layout_->addWidget(nextButton_);

    setLayout(layout_);
}

void ReviewPanel::loadCurrentCard() {
    if (cards_.isEmpty()) {
        cardLabel_->setText(tr("No cards to review."));
        for (auto* btn : optionButtons_) btn->hide();
        nextButton_->setEnabled(false);
        exitButton_->show();
        return;
    }

    const Flashcard& card = cards_.at(currentIndex_);
    nextButton_->setEnabled(true);
    exitButton_->show();

    if (card.isQuizCard()) {
        // Quiz mode
        cardLabel_->setText(card.getFrontText());
        correctAnswer_ = card.getOptions().at(card.getCorrectOptionIndex());
        for (int i = 0; i < optionButtons_.size(); ++i) {
            optionButtons_[i]->setText(card.getOptions().at(i));
            optionButtons_[i]->show();
        }
    } else {
        // Flip mode
        cardLabel_->setText(card.getFrontText());
        for (auto* btn : optionButtons_) btn->hide();
    }
}

void ReviewPanel::showNextCard() {
    if (cards_.isEmpty()) return;
    currentIndex_ = (currentIndex_ + 1) % cards_.size();
    loadCurrentCard();
}

void ReviewPanel::checkQuizAnswer() {
    QPushButton* senderBtn = qobject_cast<QPushButton*>(sender());
    if (!senderBtn) return;

    bool correct = (senderBtn->text() == correctAnswer_);
    QMessageBox::information(
        this,
        correct ? tr("Correct!") : tr("Incorrect"),
        correct ? tr("Well done!") : tr("Answer: ") + correctAnswer_
    );
}

void ReviewPanel::exitReview() {
    emit reviewExited();
}

