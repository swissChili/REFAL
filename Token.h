#pragma once

#include <QList>
#include <QChar>

class Token {
public:
    Token();
    Token(const Token &other);

    explicit Token(QChar symbol);
    explicit Token(QString &&identifier);
    explicit Token(QList<Token> parenthesized);
    Token(char varType, const QString &&name);

    bool operator ==(const Token &other) const;
    bool operator !=(const Token &other) const;

    bool isSym() const;
    bool isIdent() const;
    bool isParen() const;
    bool isVar() const;

    QList<Token> parenContent();

    char varType() const;
    const QString &name() const;

    operator QString() const;

    enum Type {
        SYM, IDENT, PAREN, VAR,
    };

    static QString typeToString(int type);

    int type() const;

private:
    int _type = 0;
    QString _stringVal = "";
    QList<Token> _listVal;
    QChar _charVal = 0;
};

using LTok = QList<Token>;
