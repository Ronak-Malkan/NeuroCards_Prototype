#ifndef CARDPREVIEWDIALOG_H
#define CARDPREVIEWDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include "../core/cardservice.h"

class CardPreviewDialog : public QDialog {
    Q_OBJECT
public:
    explicit CardPreviewDialog(CardService* cardService,
                               const QString& deckName,
                               int index,
                               QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* ev) override;

private:
    CardService* m_cardService;  // Changed from DeckManager to CardService
    QString     m_deckName;
    int         m_index;
    QLabel*     m_cardLabel;

    // Fade effect
    QGraphicsOpacityEffect* m_opacity;
    QPropertyAnimation*     m_anim;
    bool                    m_showingFront;

    void flip();
    void swapText();
};

#endif // CARDPREVIEWDIALOG_H

