#pragma once

#include <QString>
#include <QList>

#include <type_traits>

#include "Token.h"
#include "AstNode.h"
#include "Parser.h"

template <typename T>
QString pprint(T val);

//template <>
//QString pprint<Token>(Token val);

//template <>
//QString pprint<AstNode>(AstNode val);

template <typename T, typename std::enable_if_t<std::is_base_of<TokenBase<T>, T>::value>::value = true>
QString pprint(QList<T> val)
{
    QString out;
    int lastType = -1;

    qInfo() << "pprint specialized";

    for (const T &v : val)
    {
        if ((lastType != v.type() || v.type() != T::SYMBOL) && lastType != -1)
            out += " ";

        out += QString(v);
    }

    return out;
}

template <typename T>
QString pprint(QList<T> val)
{
	QStringList out;

	for (const T &v : val)
		out.append(static_cast<QString>(v));

	return out.join(" ");
}

template <typename T>
QString pprint(T val)
{
	return static_cast<QString>(val);
}

QString pprint(ParseResult val, const Parser &parser);

void sout(QString string);
void eout(QString string);
