#pragma once

#include <QList>
#include <QChar>

template <typename T>
class TokenBase
{
public:
    TokenBase();
    TokenBase(const T &other);

    explicit TokenBase(QChar symbol);
    explicit TokenBase(QString identifier);
    explicit TokenBase(QList<T> parenthesized);
    TokenBase(char varType, const QString name);

    bool operator==(const T &other) const;
    bool operator!=(const T &other) const;

    bool isSym() const;
    bool isIdent() const;
    bool isParen() const;
    bool isVar() const;

    QList<T> parenContent();

    char varType() const;
    const QString &name() const;
    QChar symbol() const;

    operator QString() const;

    enum
    {
        SYM,
        IDENT,
        PAREN,
        VAR,
        TOKEN_TYPE_LAST,
    };

    static QString typeToString(int type);

    int type() const;

protected:
    int _type = 0;
    QString _stringVal = "";
    QList<T> _listVal;
    QChar _charVal = 0;
};

class Token : public TokenBase<Token>
{
public:
    using TokenBase::TokenBase;
};

using LTok = QList<Token>;
