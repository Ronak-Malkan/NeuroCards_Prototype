#include "cardDisplayWidget.h"
#include "../core/quizcard.h"
#include <QHBoxLayout>
#include <QMouseEvent>

CardDisplayWidget::CardDisplayWidget(Flashcard* card, QWidget* parent)
  : QWidget(parent),
    card_(card),
    showingFront_(true)
{
    label_ = new QLabel(this);
    label_->setAlignment(Qt::AlignCenter);
    label_->setWordWrap(true);
    label_->setText(card_->getFrontText());

    auto *lay = new QHBoxLayout(this);
    lay->addWidget(label_);
    setLayout(lay);

    buildAnimation();
}

void CardDisplayWidget::buildAnimation() {
    // animate width → 0, swap text, width → original
    int w = width() > 0 ? width() : 200;
    auto *shrink = new QPropertyAnimation(label_, "maximumWidth");
    shrink->setDuration(150);
    shrink->setStartValue(w);
    shrink->setEndValue(0);

    auto *expand = new QPropertyAnimation(label_, "maximumWidth");
    expand->setDuration(150);
    expand->setStartValue(0);
    expand->setEndValue(w);

    flipAnim_ = new QSequentialAnimationGroup(this);
    flipAnim_->addAnimation(shrink);
    flipAnim_->addPause(50);
    QObject::connect(shrink, &QPropertyAnimation::finished, this, &CardDisplayWidget::swapText);
    flipAnim_->addAnimation(expand);
}

void CardDisplayWidget::swapText() {
    if (showingFront_) {
        if (card_->isQuizCard()) {
            // Cast to QuizCard to access quiz-specific functionality
            QuizCard* quizCard = dynamic_cast<QuizCard*>(card_);
            if (quizCard) {
                // show question + options
                QString s = quizCard->getFrontText() + "\n\n";
                auto opts = quizCard->getOptions();
                for (int i=0; i<opts.size(); ++i)
                    s += QString("%1. %2\n").arg(i+1).arg(opts[i]);
                label_->setText(s);
            }
        } else {
            label_->setText(card_->getBackText());
        }
    } else {
        label_->setText(card_->getFrontText());
    }
    showingFront_ = !showingFront_;
}

void CardDisplayWidget::mousePressEvent(QMouseEvent*) {
    if (flipAnim_->state() == QAbstractAnimation::Stopped)
        flipAnim_->start();
}

