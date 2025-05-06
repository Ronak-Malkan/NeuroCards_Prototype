#include "flipcard.h"

FlipCard::FlipCard(const QString& front, const QString& back)
    : m_front(front)
    , m_back(back)
{
}

FlipCard::~FlipCard() {
}

QString FlipCard::getFrontText() const {
    return m_front;
}

QString FlipCard::getBackText() const {
    return m_back;
}

bool FlipCard::isQuizCard() const {
    return false;
}

QJsonObject FlipCard::toJson() const {
    QJsonObject obj = Flashcard::toJson();
    obj["type"] = "flip";
    obj["front"] = m_front;
    obj["back"] = m_back;
    return obj;
}