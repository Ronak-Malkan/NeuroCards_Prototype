// src/main.cpp

#include <QApplication>
#include <QResource>          // for Q_INIT_RESOURCE
#include <QStyleFactory>
#include <QDebug>

#include "theme/ThemeFactory.h"
#include "ui/mainwindow.h"

int main(int argc, char *argv[]) {
    // Initialize the resources.qrc bundle
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);

    // Apply the Teal–Amber theme (QSS + font)
    auto theme = ThemeFactory::create("TealAmber");
    if (theme) {
        theme->apply(app);
    } else {
        qWarning() << "[Theme] no theme found!";
    }

    MainWindow w;
    w.show();
    return app.exec();
}

