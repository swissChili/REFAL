#include "Token.h"
#include "AstNode.h"

template class TokenBase<Token>;

// This is kind of ugly and breaks separation of concerns; but if I don't do
// this I have to define everything in headers which is worse.
template class TokenBase<AstNode>;

template <typename T>
TokenBase<T>::TokenBase(const T &other)
{
    _type = other._type;
    _stringVal = other._stringVal;
    _listVal = other._listVal;
    _charVal = other._charVal;
}

template <typename T>
TokenBase<T>::TokenBase(QChar symbol)
{
    _type = SYM;
    _charVal = symbol;
}

template <typename T>
TokenBase<T>::TokenBase(QString identifier)
{
    _type = IDENT;
    _stringVal = identifier;
}

template <typename T>
TokenBase<T>::TokenBase(QList<T> parenthesized)
{
    _type = PAREN;
    _listVal = std::move(parenthesized);
}

template <typename T>
TokenBase<T>::TokenBase(QString integer, int base)
{
	_type = INTEGER;
	_intVal = integer.toInt(nullptr, base);
}

template <typename T>
TokenBase<T>::TokenBase(char varType, const QString name)
{
    _type = VAR;
    _charVal = varType;
    _stringVal = name;
}

template <typename T>
bool TokenBase<T>::isSym() const
{
    return _type == SYM;
}

template <typename T>
bool TokenBase<T>::isIdent() const
{
    return _type == IDENT;
}

template <typename T>
bool TokenBase<T>::isParen() const
{
    return _type == PAREN;
}
template <typename T>
bool TokenBase<T>::isVar() const
{
    return _type == VAR;
}

template <typename T>
bool TokenBase<T>::isInteger() const
{
	return _type == INTEGER;
}

template <typename T>
TokenBase<T>::TokenBase() : TokenBase("Null")
{
}

template <typename T>
bool TokenBase<T>::operator==(const T &other) const
{
    return _type == other._type && _stringVal == other._stringVal && _charVal == other._charVal && _listVal == other._listVal;
}

template <typename T>
QList<T> TokenBase<T>::parenContent()
{
    if (isParen())
    {
        return _listVal;
    }
    else
    {
        return {};
    }
}

template <typename T>
int TokenBase<T>::integer() const
{
	return _intVal;
}

template <typename T>
char TokenBase<T>::varType() const
{
    return _charVal.toLatin1();
}

template <typename T>
const QString &TokenBase<T>::name() const
{
    return _stringVal;
}

template <typename T>
bool TokenBase<T>::operator!=(const T &other) const
{
    return !(this->operator==(other));
}

template <typename T>
TokenBase<T>::operator QString() const
{
    if (isIdent())
        return _stringVal;
    if (isSym())
        return _charVal;
    if (isVar())
        return QString(_charVal) + "." + _stringVal;
	if (isInteger())
		return QString::number(_intVal, 10);
    if (isParen())
    {
        QStringList parts;
        for (const T &tok : _listVal)
        {
            parts.append(static_cast<QString>(tok));
        }

        return "(" + parts.join(" ") + ")";
    }

    return "Null";
}

template <typename T>
int TokenBase<T>::type() const
{
    return _type;
}

template <typename T>
QString TokenBase<T>::typeToString(int type)
{
    static const QString typeNames[] = {"SYMBOL", "IDENT", "PAREN", "VAR", "INTEGER"};
    return typeNames[type];
}

template <typename T>
QChar TokenBase<T>::symbol() const
{
    return _charVal;
}
