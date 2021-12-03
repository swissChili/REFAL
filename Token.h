#pragma once

#include <QList>
#include <QChar>

class Token {
public:
    Token();
    Token(const Token &other);

    explicit Token(QChar symbol);
    explicit Token(QString &&identifier);
    explicit Token(QList<Token> &&parenthesized);
    Token(char varType, const QString &&name);

    bool operator ==(const Token &other);

    ~Token();

    bool isSym();
    bool isIdent();
    bool isParen();
    bool isVar();

    QList<Token> parenContent();

private:
    enum Type {
        SYM, IDENT, PAREN, VAR,
    };

    int _type = 0;
    QString _stringVal = "";
    QList<Token> *_listVal = nullptr;
    QChar _charVal = 0;
};
