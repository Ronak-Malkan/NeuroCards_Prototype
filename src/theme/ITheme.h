#pragma once
#include <QApplication>

class ITheme {
public:
    virtual ~ITheme() = default;
    virtual void apply(QApplication& app) = 0;
};
