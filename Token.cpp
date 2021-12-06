#include "Token.h"

#include <QDebug>
#include <utility>

Token::Token(const Token &other) {
    *this = other;
}

Token::Token(QChar symbol) {
    _type = SYM;
    _charVal = symbol;
}

Token::Token(QString &&identifier) {
    _type = IDENT;
    _stringVal = identifier;
}

Token::Token(QList<Token> parenthesized) {
    _type = PAREN;
    _listVal = std::move(parenthesized);
}

Token::Token(char varType, const QString &&name) {
    _type = VAR;
    _charVal = varType;
    _stringVal = name;
}

bool Token::isSym() const {
    return _type == SYM;
}

bool Token::isIdent() const {
    return _type == IDENT;
}

bool Token::isParen() const {
    return _type == PAREN;
}

bool Token::isVar() const {
    return _type == VAR;
}

Token::Token() : Token("Null") {
}

bool Token::operator==(const Token &other) const {
    return _type == other._type
           && _stringVal == other._stringVal
           && _charVal == other._charVal
           && _listVal == other._listVal;
}

QList<Token> Token::parenContent() {
    if (isParen()) {
        return _listVal;
    } else {
        return {};
    }
}

char Token::varType() const {
    return _charVal.toLatin1();
}

const QString &Token::name() const {
    return _stringVal;
}

bool Token::operator!=(const Token &other) const {
    return !(this->operator==(other));
}

Token::operator QString() const {
    if (isIdent())
        return _stringVal;
    if (isSym())
        return _charVal;
    if (isVar())
        return QString(_charVal) + "." + _stringVal;
    if (isParen()) {
        QStringList parts;
        for (const Token &tok : _listVal) {
            parts.append(static_cast<QString>(tok));
        }

        return "(" + parts.join(" ") + ")";
    }

    return "Null";
}

int Token::type() const {
    return _type;
}

QString Token::typeToString(int type)  {
    static const QString typeNames[] = {"SYMBOL", "IDENT", "PAREN", "VAR"};
    return typeNames[type];
}
