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

    bool operator ==(const Token &other) const;
    bool operator !=(const Token &other) const;

    bool isSym();
    bool isIdent();
    bool isParen();
    bool isVar();

    QList<Token> parenContent();

    char varType() const;
    const QString &name() const;

private:
    enum Type {
        SYM, IDENT, PAREN, VAR,
    };

    int _type = 0;
    QString _stringVal = "";
    QList<Token> _listVal;
    QChar _charVal = 0;
};

using LTok = QList<Token>;
