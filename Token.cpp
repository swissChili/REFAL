#include "Token.h"

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

Token::Token(QList<Token> &&parenthesized) {
    _type = PAREN;
    _listVal = parenthesized;
}

Token::Token(char varType, const QString &&name) {
    _type = VAR;
    _charVal = varType;
    _stringVal = name;
}

bool Token::isSym() {
    return _type == SYM;
}

bool Token::isIdent() {
    return _type == IDENT;
}

bool Token::isParen() {
    return _type == PAREN;
}

bool Token::isVar() {
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
