#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QString>
#include <QJsonObject>

class Flashcard {
public:
    Flashcard(const QString &front = "", const QString &back = "");

    QString getFront() const;
    QString getBack() const;

    QJsonObject toJson() const;
    static Flashcard fromJson(const QJsonObject &json);

private:
    QString m_front;
    QString m_back;
};

#endif // FLASHCARD_H
