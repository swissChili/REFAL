#pragma once

#include <QString>

#include "AstNode.h"

class Parser
{
public:
    explicit Parser(QString input);

    QChar peek();
    QChar get();
    bool atEnd();

    void skip();

    bool parseSymbol(AstNode *node);
    bool parseIdentifier(AstNode *node);
    bool parseNumber(AstNode *node);
    bool parseVariable(AstNode *node);
    bool parseParens(AstNode *node);
    bool parseFunction(AstNode *node);
    QList<AstNode> parseMany();
    bool parseOne(AstNode *node);

private:
    int _pos = 0;
    QString _input;
};
