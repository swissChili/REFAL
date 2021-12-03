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
    _listVal = new QList<Token>(parenthesized);
}

Token::Token(char varType, const QString &&name) {
    _type = VAR;
    _charVal = varType;
    _stringVal = name;
}

Token::~Token() {
    // Стерать нулевые пойнтеры безопасно
    delete _listVal;
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

bool Token::operator==(const Token &other) {
    return _type == other._type
           && _stringVal == other._stringVal
           && _charVal == other._charVal
           && (_listVal == nullptr || *_listVal == (*other._listVal));
}

QList<Token> Token::parenContent() {
    if (isParen() && _listVal) {
        return *_listVal;
    } else {
        return {};
    }
}
