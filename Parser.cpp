#include "Parser.h"

#include <QDebug>

Parser::Parser(QString input)
{
    _input = input;
}

QChar Parser::peek()
{
    if (atEnd())
        return 0;

    return _input[_pos];
}

QChar Parser::get()
{
    return _input[_pos++];
}

bool Parser::atEnd()
{
    return _pos >= _input.length();
}

void Parser::skip()
{
    while (peek().isSpace())
        get();
}

bool Parser::parseSymbol(AstNode *node)
{
    skip();

    if (peek().isLetter())
    {
        *node = AstNode(get());
        return true;
    }

    return false;
}

bool Parser::parseIdentifier(AstNode *node)
{
    skip();

    QString buffer;

    if (peek().isUpper())
    {
        while (peek().isLetter() || peek() == '-' || peek() == '_' || peek().isNumber())
        {
            buffer += get();
        }

        *node = AstNode(buffer);
        return true;
    }

    return false;
}

bool Parser::parseNumber(AstNode *node)
{
    skip();

    QString buffer;

    if (peek().isDigit())
    {
        while (peek().isDigit())
            buffer += get();

        *node = AstNode(buffer.toInt());
        return true;
    }

    return false;
}

bool Parser::parseVariable(AstNode *node)
{
    skip();

    int pos = _pos;

    if (peek() == 's' || peek() == 'e' || peek() == 't')
    {
        char type = get().toLatin1();

        if (peek() == '.')
        {
            get();

            AstNode identNode;

            if (parseIdentifier(&identNode))
            {
                *node = AstNode(type, identNode.name());
                return true;
            }
        }
    }

    _pos = pos;
    return false;
}

bool Parser::parseOne(AstNode *node)
{
    return parseFunction(node) ||
           parseVariable(node) ||
           parseNumber(node) ||
           parseIdentifier(node) ||
           parseSymbol(node) ||
           parseParens(node);
}

QList<AstNode> Parser::parseMany()
{
    QList<AstNode> nodes;
    AstNode next;

    while (parseOne(&next))
    {
        nodes.append(next);
    }

    return nodes;
}

bool Parser::parseParens(AstNode *node)
{
    skip();

    int pos = _pos;

    if (peek() != '(')
        return false;

    get();

    QList<AstNode> many = parseMany();
    *node = AstNode(many);

    skip();
    if (peek() != ')')
    {
        _pos = pos;
        return false;
    }

    get();

    return true;
}

bool Parser::parseFunction(AstNode *node)
{
    skip();

    int pos = _pos;

    if (peek() != '<')
        return false;

    get();

    AstNode head;
    if (!parseIdentifier(&head))
    {
        _pos = pos;
        return false;
    }

    QList<AstNode> body = parseMany();
    *node = AstNode(head.name(), body);

    skip();
    if (peek() != '>')
    {
        _pos = pos;
        return false;
    }

    get();

    return true;
}
