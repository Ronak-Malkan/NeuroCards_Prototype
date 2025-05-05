#include "flashcard.h"
#include <QJsonObject>
#include <QJsonArray>

// Flip card
Flashcard::Flashcard(const QString& front, const QString& back)
    : m_front(front),
    m_back(back),
    m_options(),
    m_correctIndex(-1),
    m_attempts(0),
    m_correctCount(0)
{
}

// Quiz card
Flashcard::Flashcard(const QString& front, const QStringList& options, int correctIndex)
    : m_front(front),
    m_back(QString()),
    m_options(options),
    m_correctIndex(correctIndex),
    m_attempts(0),
    m_correctCount(0)
{
}

bool Flashcard::isQuizCard() const {
    return m_correctIndex >= 0 && m_correctIndex < m_options.size();
}

QString Flashcard::getFrontText() const {
    return m_front;
}

QString Flashcard::getBackText() const {
    return m_back;
}

QStringList Flashcard::getOptions() const {
    return m_options;
}

int Flashcard::getCorrectOptionIndex() const {
    return m_correctIndex;
}

// Stats
void Flashcard::recordResult(bool correct) {
    m_attempts++;
    if (correct) m_correctCount++;
}

int Flashcard::getAttempts() const { return m_attempts; }
int Flashcard::getCorrectCount() const { return m_correctCount; }

// JSON
QJsonObject Flashcard::toJson() const {
    QJsonObject obj;
    if (isQuizCard()) {
        obj["type"] = "quiz";
        obj["front"] = m_front;
        QJsonArray arr;
        for (const QString& opt : m_options)
            arr.append(opt);
        obj["options"] = arr;
        obj["correctIndex"] = m_correctIndex;
    } else {
        obj["type"] = "flip";
        obj["front"] = m_front;
        obj["back"] = m_back;
    }
    // Add stats
    QJsonObject stats;
    stats["attempts"] = m_attempts;
    stats["correctCount"] = m_correctCount;
    obj["stats"] = stats;
    return obj;
}

Flashcard Flashcard::fromJson(const QJsonObject& obj) {
    QString type = obj.value("type").toString();
    Flashcard card = [&]() {
        if (type == "quiz") {
            QString front = obj.value("front").toString();
            QJsonArray arr = obj.value("options").toArray();
            QStringList options;
            for (const QJsonValue& v : arr)
                options.append(v.toString());
            int correct = obj.value("correctIndex").toInt();
            return Flashcard(front, options, correct);
        } else {
            QString front = obj.value("front").toString();
            QString back  = obj.value("back").toString();
            return Flashcard(front, back);
        }
    }();
    // Read stats if present
    if (obj.contains("stats") && obj.value("stats").isObject()) {
        QJsonObject s = obj.value("stats").toObject();
        card.m_attempts     = s.value("attempts").toInt(0);
        card.m_correctCount = s.value("correctCount").toInt(0);
    }
    return card;
}
