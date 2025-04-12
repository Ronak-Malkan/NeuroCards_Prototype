#include "flashcard.h"

Flashcard::Flashcard(const QString &front, const QString &back)
    : m_front(front), m_back(back)
{}

QString Flashcard::getFront() const {
    return m_front;
}

QString Flashcard::getBack() const {
    return m_back;
}

QJsonObject Flashcard::toJson() const {
    QJsonObject obj;
    obj["front"] = m_front;
    obj["back"] = m_back;
    return obj;
}

Flashcard Flashcard::fromJson(const QJsonObject &json) {
    return Flashcard(json["front"].toString(), json["back"].toString());
}
