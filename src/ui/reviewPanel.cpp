#include "reviewPanel.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include <algorithm>

ReviewPanel::ReviewPanel(DeckManager* manager, QWidget *parent)
    : QWidget(parent), deckManager_(manager), currentIndex_(0), showingBack_(false) {

    cards_ = deckManager_->getFlashcards().toVector();
    std::shuffle(cards_.begin(), cards_.end(), QRandomGenerator::global()->generate());

    setupUI();
    loadCurrentCard();
}

void ReviewPanel::setupUI() {
    layout_ = new QVBoxLayout(this);

    cardLabel_ = new QLabel(this);
    cardLabel_->setAlignment(Qt::AlignCenter);
    cardLabel_->setWordWrap(true);
    layout_->addWidget(cardLabel_);

    flipButton_ = new QPushButton("Flip", this);
    connect(flipButton_, &QPushButton::clicked, this, &ReviewPanel::flipCard);

    for (int i = 0; i < 4; ++i) {
        QPushButton* btn = new QPushButton(this);
        connect(btn, &QPushButton::clicked, this, &ReviewPanel::checkQuizAnswer);
        btn->hide();  // shown only for quiz cards
        optionButtons_.append(btn);
        layout_->addWidget(btn);
    }

    nextButton_ = new QPushButton("Next", this);
    connect(nextButton_, &QPushButton::clicked, this, &ReviewPanel::showNextCard);
    layout_->addWidget(nextButton_);

    exitButton_ = new QPushButton("Exit Review Mode", this);
    connect(exitButton_, &QPushButton::clicked, this, &ReviewPanel::exitReview);
    layout_->addWidget(exitButton_);

    layout_->addWidget(flipButton_);
    setLayout(layout_);
}

void ReviewPanel::loadCurrentCard() {
    clearLayout(layout_);
    layout_->addWidget(cardLabel_);
    layout_->addWidget(flipButton_);
    for (QPushButton* btn : optionButtons_) layout_->addWidget(btn);
    layout_->addWidget(nextButton_);
    layout_->addWidget(exitButton_);

    if (cards_.isEmpty()) {
        cardLabel_->setText("No cards to review.");
        flipButton_->hide();
        for (auto* btn : optionButtons_) btn->hide();
        return;
    }

    const Flashcard& card = cards_[currentIndex_];

    if (card.isQuizCard()) {
        cardLabel_->setText(card.getFrontText());
        flipButton_->hide();

        QStringList opts = card.getOptions();
        correctAnswer_ = opts[card.getCorrectOptionIndex()];

        for (int i = 0; i < 4; ++i) {
            optionButtons_[i]->setText(opts[i]);
            optionButtons_[i]->show();
        }
    } else {
        cardLabel_->setText(card.getFrontText());
        showingBack_ = false;
        flipButton_->show();
        for (auto* btn : optionButtons_) btn->hide();
    }
}

void ReviewPanel::showNextCard() {
    if (cards_.isEmpty()) return;

    currentIndex_ = (currentIndex_ + 1) % cards_.size();
    loadCurrentCard();
}

void ReviewPanel::flipCard() {
    if (cards_.isEmpty()) return;

    const Flashcard& card = cards_[currentIndex_];
    cardLabel_->setText(showingBack_ ? card.getFrontText() : card.getBackText());
    showingBack_ = !showingBack_;
}

void ReviewPanel::checkQuizAnswer() {
    QPushButton* senderBtn = qobject_cast<QPushButton*>(sender());
    if (!senderBtn) return;

    QString selected = senderBtn->text();
    bool correct = (selected == correctAnswer_);
    QMessageBox::information(this, correct ? "Correct!" : "Incorrect",
                             correct ? "Well done!" : "Correct answer: " + correctAnswer_);
}

void ReviewPanel::exitReview() {
    emit reviewExited();
}

void ReviewPanel::clearLayout(QLayout* layout) {
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (item->widget() && item->widget() != modeSelector_) {
            item->widget()->hide();  // hide instead of delete
        }
        delete item;
    }
}

