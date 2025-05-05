#include "cardPreviewDialog.h"
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QtGlobal>

CardPreviewDialog::CardPreviewDialog(DeckManager* manager,
                                     const QString& deckName,
                                     int index,
                                     QWidget* parent)
  : QDialog(parent),
    m_deckManager(manager),
    m_deckName(deckName),
    m_index(index),
    m_showingFront(true)
{
    setWindowTitle(tr("Card Preview"));
    resize(400, 200);

    m_cardLabel = new QLabel(this);
    m_cardLabel->setAlignment(Qt::AlignCenter);
    m_cardLabel->setWordWrap(true);
    QFont f = m_cardLabel->font();
    f.setPointSize(14);
    m_cardLabel->setFont(f);

    m_opacity = new QGraphicsOpacityEffect(m_cardLabel);
    m_cardLabel->setGraphicsEffect(m_opacity);

    m_anim = new QPropertyAnimation(m_opacity, "opacity", this);
    m_anim->setDuration(200);
    connect(m_anim, &QPropertyAnimation::finished, this, &CardPreviewDialog::swapText);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_cardLabel);
    setLayout(layout);

    // show the front text initially
    const auto& card = m_deckManager->getFlashcards(m_deckName).at(m_index);
    m_cardLabel->setText(card.getFrontText());
}

void CardPreviewDialog::mousePressEvent(QMouseEvent* ev) {
    Q_UNUSED(ev);
    flip();
}

void CardPreviewDialog::flip() {
    m_anim->stop();
    // fade out
    m_anim->setStartValue(1.0);
    m_anim->setEndValue(0.0);
    m_anim->start();
}

void CardPreviewDialog::swapText() {
    // Only handle fade-out. When fade-in finishes, opacity will be 1.0,
    // so we skip at that point.
    if (!qFuzzyCompare(m_opacity->opacity(), 0.0))
        return;

    // Swap the text
    const auto& card = m_deckManager->getFlashcards(m_deckName).at(m_index);
    if (m_showingFront) {
        // Show back / correct answer…
        if (card.isQuizCard()) {
            QString txt;
            for (int i = 0; i < card.getOptions().size(); ++i)
                txt += QString("%1. %2\n")
                           .arg(i+1)
                           .arg(card.getOptions().at(i));
            txt += QString("\nAnswer: %1")
                       .arg(card.getOptions().at(card.getCorrectOptionIndex()));
            m_cardLabel->setText(txt);
        } else {
            m_cardLabel->setText(card.getBackText());
        }
    } else {
        // Show front again
        m_cardLabel->setText(card.getFrontText());
    }
    m_showingFront = !m_showingFront;

    // Now fade back in
    m_anim->stop();
    m_anim->setStartValue(0.0);
    m_anim->setEndValue(1.0);
    m_anim->start();
}
