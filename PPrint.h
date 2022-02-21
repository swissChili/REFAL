#pragma once

#include <QString>
#include <QList>

#include "Token.h"
#include "AstNode.h"
#include "Parser.h"

template <typename T>
QString pprint(T val);

template <>
QString pprint<Token>(Token val);

template <>
QString pprint<AstNode>(AstNode val);

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
