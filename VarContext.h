#pragma once

#include <QMap>
#include "Token.h"

class VarContext {
public:
    VarContext() = default;
    VarContext(VarContext const &other)  noexcept;

    void add(char t, const QString &&name, const Token &value);
    char exists(const QString &name);
    Token operator [](const QString &name);

private:
    struct Var {
        Var() = default;

        char t = 0;
        Token value;
    };

    QMap<QString, Var> _vars;
};