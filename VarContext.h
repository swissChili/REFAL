#pragma once

#include <QMap>
#include <QDebug>
#include "Token.h"

class VarContext
{
public:
    VarContext() = default;
    VarContext(VarContext const &other)  noexcept;

    void add(char t, const QString &name, const Token &value);
    void add(char t, const QString &name, const QList<Token> &value);

    char exists(const QString &name);
    Token singleVar(const QString &name);
    QList<Token> expressionVar(const QString &name);

    friend QDebug &operator <<(QDebug &debug, const VarContext &ctx);

private:
    struct Var
    {
        Var() = default;

        char t = 0;
        Token value;
        QList<Token> expressionValue;
    };

    QMap<QString, Var> _vars;
};

QDebug &operator <<(QDebug &debug, const VarContext &ctx);
