#pragma once

#include <QString>
#include <QList>

#include <type_traits>

#include "Token.h"
#include "AstNode.h"
#include "Parser.h"

template <typename T>
QString pprint(T val);

QString pprint(QList<Token> val);
QString pprint(QList<AstNode> val);

template <typename T>
QString pprintDense(QList<T> val)
{
    QString out;
    int lastType = -1;

    qInfo() << "pprint specialized";

    for (const T &v : val)
    {
        if ((lastType != v.type() || v.type() != T::SYM) && lastType != -1)
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

class PPrint
{
public:
    enum Style
    {
        ANSI,
        HTML
    };
};

QString pprint(ParseResult val, const Parser &parser, PPrint::Style style = PPrint::ANSI);

void sout(QString string);
void eout(QString string);
