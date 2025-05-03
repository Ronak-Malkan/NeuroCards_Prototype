#include "cardPreviewDialog.h"
#include "cardDisplayWidget.h"
#include <QVBoxLayout>

CardPreviewDialog::CardPreviewDialog(const Flashcard& card, QWidget* parent)
  : QDialog(parent)
{
    setWindowTitle("Card Preview");
    auto *lay = new QVBoxLayout(this);
    auto *disp = new CardDisplayWidget(card, this);
    disp->setMinimumSize(300, 200);
    lay->addWidget(disp);

    setLayout(lay);
    setModal(true);
}

