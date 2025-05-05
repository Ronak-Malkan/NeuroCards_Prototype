#ifndef CARDPREVIEWDIALOG_H
#define CARDPREVIEWDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include "../core/deckmanager.h"
#include "../core/flashcard.h"

class CardPreviewDialog : public QDialog {
    Q_OBJECT
public:
    explicit CardPreviewDialog(DeckManager* manager,
                               const QString& deckName,
                               int index,
                               QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* ev) override;

private slots:
    void swapText();

private:
    DeckManager*          m_deckManager;
    QString               m_deckName;
    int                   m_index;
    QLabel*               m_cardLabel;
    bool                  m_showingFront;
    QGraphicsOpacityEffect* m_opacity;
    QPropertyAnimation*     m_anim;

    void flip();
};

#endif // CARDPREVIEWDIALOG_H

