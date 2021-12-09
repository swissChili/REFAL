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

template <typename T>
bool Parser::parseSymbol(T *node)
{
    skip();

    if (peek().isLetter())
    {
        *node = T(get());
        return true;
    }

    return false;
}

template <typename T>
bool Parser::parseIdentifier(T *node)
{
    skip();

    QString buffer;

    if (peek().isUpper())
    {
        while (peek().isLetter() || peek() == '-' || peek() == '_' || peek().isNumber())
        {
            buffer += get();
        }

        *node = T(buffer);
        return true;
    }

    return false;
}

template <typename T>
bool Parser::parseNumber(T *node)
{
    skip();

    QString buffer;

    if (peek().isDigit())
    {
        while (peek().isDigit())
            buffer += get();

        *node = T::fromInteger(buffer.toInt());
        return true;
    }

    return false;
}

template <typename T>
bool Parser::parseVariable(T *node)
{
    skip();

    int pos = _pos;

    if (peek() == 's' || peek() == 'e' || peek() == 't')
    {
        char type = get().toLatin1();

        if (peek() == '.')
        {
            get();

            T nameNode;

            if (parseIdentifier(&nameNode))
            {
                *node = T(type, nameNode.name());
                return true;
            }
            else if (parseSymbol(&nameNode))
            {
                *node = T(type, QString(nameNode.symbol()));
                return true;
            }
        }
    }

    _pos = pos;
    return false;
}

template <typename T>
QList<T> Parser::parseMany()
{
    QList<T > nodes;
    T next;

    while (parseOne(&next))
    {
        nodes.append(next);
    }

    return nodes;
}

template <typename T>
bool Parser::parseParens(T *node)
{
    skip();

    int pos = _pos;

    if (peek() != '(')
        return false;

    get();

    QList<T> many = parseMany<T>();
    *node = T(many);

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

    QList<AstNode> body = parseMany<AstNode>();
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

template <>
bool Parser::parseOne<Token>(Token *node)
{
    return parseVariable(node) ||
           parseNumber(node) ||
           parseIdentifier(node) ||
           parseSymbol(node) ||
           parseParens(node);
}

template <>
bool Parser::parseOne<AstNode>(AstNode *node)
{
    return parseFunction(node) ||
           parseVariable(node) ||
           parseNumber(node) ||
           parseIdentifier(node) ||
           parseSymbol(node) ||
           parseParens(node);
}

bool Parser::parseSentence(Sentence *sentence)
{
    int pos = _pos;

    QList<Token> pattern = parseMany<Token>();

    skip();

    if (get() != '=')
    {
        _pos = pos;
        return false;
    }

    QList<AstNode> result = parseMany<AstNode>();

    skip();

    if (get() != ';')
    {
        _pos = pos;
        return false;
    }

    *sentence = Sentence(pattern, result);
    return true;
}

bool Parser::parseFunctionDefinition(Function *function)
{
    int pos = _pos;

    Token identifier;
    if (!parseIdentifier(&identifier))
    {
        _pos = pos;
        return false;
    }

    QString name = identifier.name();
    Function func(name);

    skip();

    if (get() != '{')
    {
        _pos = pos;
        return false;
    }

    Sentence sentence;
    while (parseSentence(&sentence))
    {
        func.addSentence(sentence);
        skip();
    }

    if (get() != '}')
    {
        _pos = pos;
        return false;
    }

    *function = func;
    return true;
}
