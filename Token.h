#pragma once

#include <QList>
#include <QChar>

template <typename T>
class TokenBase
{
public:
    TokenBase();
    TokenBase(const T &other);

	TokenBase(QString integer, int base);
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
	bool isInteger() const;

    QList<T> parenContent();

    char varType() const;
    const QString &name() const;
    QChar symbol() const;
	int integer() const;

    operator QString() const;

    enum
    {
        SYM,
        IDENT,
        PAREN,
        VAR,
		INTEGER,
        TOKEN_TYPE_LAST,
    };

    static QString typeToString(int type);

    int type() const;

protected:
    int _type = 0;
	int _intVal = 0;
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
