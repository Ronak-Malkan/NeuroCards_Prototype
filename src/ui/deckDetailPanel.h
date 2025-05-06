#ifndef DECKDETAILPANEL_H
#define DECKDETAILPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "../core/deckmanager.h"

class DeckDetailPanel : public QWidget {
    Q_OBJECT
public:
    explicit DeckDetailPanel(DeckManager* manager, QWidget* parent = nullptr);

    // Set which deck to display
    void setDeck(const QString& deckName);
    // Refresh list of cards in the current deck
    void refreshList();

signals:
    void studyAll(const QString& deckName);
    void studyDue(const QString& deckName);
    void backToDeckList();
    void addCardRequested(const QString& deckName);

private slots:
    void onBackClicked();
    void onStatsClicked();
    void onAddCardClicked();
    void onItemClicked(QListWidgetItem* item);
    void onListContextMenu(const QPoint &pos);

private:
    DeckManager*    m_deckManager;
    QString         m_deckName;
    QPushButton*    m_backButton;
    QLabel*         m_titleLabel;
    QLabel*         m_countLabel;
    QPushButton*    m_statsButton;
    QPushButton*    m_studyAllButton;
    QPushButton*    m_studyDueButton;
    QPushButton*    m_addCardButton;
    QListWidget*    m_listWidget;
};

#endif // DECKDETAILPANEL_H

