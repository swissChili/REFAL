#pragma once

#include <QMap>
#include "Token.h"

class VarContext {
public:
    VarContext() = default;
    VarContext(VarContext const &other)  noexcept;

    void add(char t, const QString &name, const Token &value);
    void add(char t, const QString &name, const QList<Token> &value);

    char exists(const QString &name);
    Token singleVar(const QString &name);
    QList<Token> expressionVar(const QString &name);

private:
    struct Var {
        Var() = default;

        char t = 0;
        Token value;
        QList<Token> expressionValue;
    };

    QMap<QString, Var> _vars;
};