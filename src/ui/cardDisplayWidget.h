#ifndef CARDDISPLAYWIDGET_H
#define CARDDISPLAYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include "../core/flashcard.h"

class CardDisplayWidget : public QWidget {
    Q_OBJECT
public:
    explicit CardDisplayWidget(const Flashcard& card, QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* ev) override;

private:
    Flashcard card_;
    bool showingFront_;

    QLabel* label_;
    QSequentialAnimationGroup* flipAnim_;

    void buildAnimation();
    void swapText();
};

#endif // CARDDISPLAYWIDGET_H

