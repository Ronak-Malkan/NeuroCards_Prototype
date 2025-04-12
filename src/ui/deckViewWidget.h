#ifndef DECKVIEWWIDGET_H
#define DECKVIEWWIDGET_H

#include <QWidget>

class QListWidget;
class DeckManager;

class DeckViewWidget : public QWidget {
    Q_OBJECT
public:
    explicit DeckViewWidget(DeckManager *deckManager, QWidget *parent = nullptr);
    void refreshView();

private:
    QListWidget *m_listWidget;
    DeckManager *m_deckManager;
};

#endif // DECKVIEWWIDGET_H
