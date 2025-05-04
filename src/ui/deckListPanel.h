#ifndef DECKLISTPANEL_H
#define DECKLISTPANEL_H

#include <QWidget>
#include <QStringList>
#include <QListWidget>
#include <QPushButton>
#include "../core/deckmanager.h"

class DeckListPanel : public QWidget {
    Q_OBJECT

public:
    explicit DeckListPanel(DeckManager* manager, QWidget* parent = nullptr);

signals:
    void deckSelected(const QString& name);

private slots:
    void onAddDeck();
    void onItemClicked(QListWidgetItem* item);
    void onContextMenuRequested(const QPoint& pos);
    void renameDeck();
    void deleteDeck();

private:
    DeckManager* m_deckManager;
    QListWidget* m_listWidget;
    QPushButton* m_addButton;

    void refreshList();
};

#endif // DECKLISTPANEL_H

