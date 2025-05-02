#include "flashcard.h"
#include <QJsonObject>
#include <QJsonArray>

// Flip card constructor
Flashcard::Flashcard(const QString& front, const QString& back)
    : frontText_(front), backText_(back), type_(CardType::Flip), correctOptionIndex_(-1) {}

// Quiz card constructor
Flashcard::Flashcard(const QString& question, const QStringList& options, int correctIndex)
    : frontText_(question), backText_(""), type_(CardType::Quiz),
      options_(options), correctOptionIndex_(correctIndex) {}

QString Flashcard::getFrontText() const {
    return frontText_;
}

QString Flashcard::getBackText() const {
    return backText_;
}

Flashcard::CardType Flashcard::getType() const {
    return type_;
}

QStringList Flashcard::getOptions() const {
    return options_;
}

int Flashcard::getCorrectOptionIndex() const {
    return correctOptionIndex_;
}

bool Flashcard::isQuizCard() const {
    return type_ == CardType::Quiz;
}

QJsonObject Flashcard::toJson() const {
    QJsonObject obj;
    obj["type"] = (type_ == CardType::Quiz) ? "quiz" : "flip";
    obj["front"] = frontText_;
    obj["back"] = backText_;

    if (type_ == CardType::Quiz) {
        QJsonArray opts;
        for (const QString& opt : options_) {
            opts.append(opt);
        }
        obj["options"] = opts;
        obj["correctIndex"] = correctOptionIndex_;
    }

    return obj;
}

Flashcard Flashcard::fromJson(const QJsonObject& obj) {
    QString typeStr = obj["type"].toString();
    QString front = obj["front"].toString();
    QString back = obj["back"].toString();

    if (typeStr == "quiz") {
        QJsonArray optsArray = obj["options"].toArray();
        QStringList options;
        for (const QJsonValue& val : optsArray) {
            options << val.toString();
        }
        int correctIdx = obj["correctIndex"].toInt();
        return Flashcard(front, options, correctIdx);
    } else {
        return Flashcard(front, back);
    }
}
