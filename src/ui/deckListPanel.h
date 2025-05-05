#ifndef DECKLISTPANEL_H
#define DECKLISTPANEL_H

#include <QWidget>
#include <QString>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include "../core/deckmanager.h"

class DeckListPanel : public QWidget {
    Q_OBJECT
public:
    explicit DeckListPanel(DeckManager* manager, QWidget* parent = nullptr);

signals:
    void deckSelected(const QString& deckName);
    void createDeckRequested();  // existing create deck signal

private slots:
    void onSearchTextChanged(const QString& text);
    void onItemClicked(QListWidgetItem* item);
    void onListContextMenu(const QPoint &pos);

private:
    DeckManager* m_deckManager;
    QLineEdit*   m_searchInput;
    QListWidget* m_listWidget;
    QPushButton* m_addDeckButton;

    void setupUI();
    void connectSignals();
    void refreshList();
};

#endif // DECKLISTPANEL_H
