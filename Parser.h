#pragma once

#include <QString>

#include "Token.h"
#include "AstNode.h"

class Parser
{
public:
    explicit Parser(QString input);

    QChar peek();
    QChar get();
    bool atEnd();

    void skip();

    template <typename T>
    bool parseSymbol(T *node);

    template <typename T>
    bool parseIdentifier(T *node);

    template <typename T>
    bool parseNumber(T *node);

    template <typename T>
    bool parseVariable(T *node);

    template <typename T>
    bool parseParens(T *node);

    bool parseFunction(AstNode *node);

    template <typename T>
    QList<T> parseMany();

    template <typename T>
    bool parseOne(T *node);

private:
    int _pos = 0;
    QString _input;
};

template <>
bool Parser::parseOne<Token>(Token *node);

template <>
bool Parser::parseOne<AstNode>(AstNode *node);
