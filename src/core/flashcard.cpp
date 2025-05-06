#include "flashcard.h"
#include "flipcard.h"
#include "quizcard.h"
#include <QJsonArray>
#include <QDebug>

Flashcard::Flashcard()
    : m_attempts(0)
    , m_correctCount(0)
    , m_repetition(0)
    , m_interval(1)
    , m_easeFactor(2.5)
    , m_nextReview(QDate::currentDate())
{
}

Flashcard::~Flashcard() {
}

void Flashcard::recordResult(int quality) {
    // Basic stats
    m_attempts++;
    if (quality >= 3) m_correctCount++;

    qDebug() << "[DEBUG MODEL] recordResult front='" << getFrontText()
         << "' quality=" << quality
         << " → rep="    << m_repetition
         << " int="      << m_interval
         << " ef="       << m_easeFactor
         << " attempts=" << m_attempts
         << " correct="  << m_correctCount;

    // SM-2 scheduling
    if (quality < 3) {
        m_repetition = 0;
        m_interval   = 1;
    } else {
        m_repetition++;
        if (m_repetition == 1)
            m_interval = 1;
        else if (m_repetition == 2)
            m_interval = 6;
        else
            m_interval = qRound(m_interval * m_easeFactor);

        m_easeFactor += (0.1 - (5 - quality) * (0.08 + (5 - quality) * 0.02));
        if (m_easeFactor < 1.3) m_easeFactor = 1.3;
    }

    m_nextReview = QDate::currentDate().addDays(m_interval);
}

int Flashcard::getAttempts() const    { return m_attempts;    }
int Flashcard::getCorrectCount() const { return m_correctCount; }

// Spaced repetition getters
int Flashcard::getRepetition() const   { return m_repetition;   }
int Flashcard::getInterval() const     { return m_interval;     }
double Flashcard::getEaseFactor() const { return m_easeFactor;   }
QDate Flashcard::getNextReview() const  { return m_nextReview;  }

void Flashcard::populateJson(QJsonObject& obj) const {
    // Stats
    QJsonObject stats;
    stats["attempts"] = m_attempts;
    stats["correctCount"] = m_correctCount;
    obj["stats"] = stats;

    // Scheduling
    QJsonObject sched;
    sched["repetition"] = m_repetition;
    sched["interval"] = m_interval;
    sched["easeFactor"] = m_easeFactor;
    sched["nextReview"] = m_nextReview.toString(Qt::ISODate);
    obj["schedule"] = sched;
}

void Flashcard::populateFromJson(const QJsonObject& obj) {
    // Stats
    if (obj.contains("stats") && obj.value("stats").isObject()) {
        QJsonObject s = obj.value("stats").toObject();
        m_attempts = s.value("attempts").toInt(0);
        m_correctCount = s.value("correctCount").toInt(0);
    }
    
    // Scheduling
    if (obj.contains("schedule") && obj.value("schedule").isObject()) {
        QJsonObject s = obj.value("schedule").toObject();
        m_repetition = s.value("repetition").toInt(0);
        m_interval = s.value("interval").toInt(1);
        m_easeFactor = s.value("easeFactor").toDouble(2.5);
        m_nextReview = QDate::fromString(
            s.value("nextReview").toString(), Qt::ISODate);
    }
}

QJsonObject Flashcard::toJson() const {
    QJsonObject obj;
    populateJson(obj);
    return obj;
}

Flashcard* Flashcard::fromJson(const QJsonObject& obj) {
    QString type = obj.value("type").toString();
    Flashcard* card = nullptr;
    
    if (type == "quiz") {
        QString front = obj.value("front").toString();
        QJsonArray arr = obj.value("options").toArray();
        QStringList options;
        for (const QJsonValue& v : arr)
            options.append(v.toString());
        int correct = obj.value("correctIndex").toInt();
        card = new QuizCard(front, options, correct);
    } else {
        QString front = obj.value("front").toString();
        QString back = obj.value("back").toString();
        card = new FlipCard(front, back);
    }
    
    card->populateFromJson(obj);
    return card;
}
