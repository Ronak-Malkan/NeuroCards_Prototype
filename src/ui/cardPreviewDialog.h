#ifndef CARDPREVIEWDIALOG_H
#define CARDPREVIEWDIALOG_H

#include <QDialog>
#include "../core/flashcard.h"

class CardPreviewDialog : public QDialog {
    Q_OBJECT
public:
    explicit CardPreviewDialog(const Flashcard& card, QWidget* parent = nullptr);
};

#endif // CARDPREVIEWDIALOG_H

