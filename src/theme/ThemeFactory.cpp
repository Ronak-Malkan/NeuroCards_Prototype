#include "ThemeFactory.h"
#include "TealAmberTheme.h"

std::unique_ptr<ITheme> ThemeFactory::create(const QString& name) {
    if (name == "TealAmber") {
        return std::make_unique<TealAmberTheme>();
    }
    // future themes can be added here
    return nullptr;
}
