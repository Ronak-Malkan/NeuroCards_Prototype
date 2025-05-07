#pragma once
#include "ITheme.h"

class TealAmberTheme : public ITheme {
public:
    void apply(QApplication& app) override;
};
