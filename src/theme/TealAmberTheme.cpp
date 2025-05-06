// src/theme/TealAmberTheme.cpp

#include "TealAmberTheme.h"
#include <QFile>
#include <QFileInfo>
#include <QFontDatabase>
#include <QApplication>
#include <QStyleFactory>
#include <QDebug>

void TealAmberTheme::apply(QApplication& app) {
    // 0) Force Fusion so QSS is honored everywhere
    app.setStyle(QStyleFactory::create("Fusion"));

    // 1) Load & register custom font
    int fontId = QFontDatabase::addApplicationFont(":../fonts/OpenSans-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(fontId).value(0);
    constexpr int kBaseFontPt = 14;                        // bump up from 10 to 14
    QFont font(family, kBaseFontPt);
    app.setFont(font);

    QFileInfo info(":/themes/themes/teal-amber.qss");
    qDebug() << "[Theme] resource exists? " << info.exists();

    // 2) Load and apply QSS
    QFile qssFile(":/themes/themes/teal-amber.qss");
    if (qssFile.open(QFile::ReadOnly | QFile::Text)) {
        QString style = qssFile.readAll();
        qDebug() << "[Theme] loaded QSS, length =" << style.length();
        app.setStyleSheet(style);
        qssFile.close();
    } else {
        qWarning() << "[Theme] failed to open QSS:" << qssFile.errorString();
    }
}

