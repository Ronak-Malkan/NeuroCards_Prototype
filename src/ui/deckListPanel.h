#ifndef DECKLISTPANEL_H
#define DECKLISTPANEL_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include "../core/cardservice.h"

class DeckListPanel : public QWidget {
    Q_OBJECT

public:
    explicit DeckListPanel(CardService* cardService, QWidget* parent = nullptr);

signals:
    void deckSelected(const QString& deckName);

private slots:
    void onAddDeckClicked();
    void onDeleteDeckClicked();
    void onRenameDeckClicked();
    void onItemClicked(QListWidgetItem* item);
    void onListContextMenu(const QPoint& pos);
    void refreshList();

private:
    CardService* m_cardService;  // Changed from DeckManager to CardService
    QListWidget* m_listWidget;
    QPushButton* m_addDeckButton;
    QPushButton* m_deleteDeckButton;
    QPushButton* m_renameDeckButton;
};

#endif // DECKLISTPANEL_H
