#pragma once
#include <memory>
#include <QString>
#include "ITheme.h"

class ThemeFactory {
public:
    static std::unique_ptr<ITheme> create(const QString& name);
};
