#pragma once

#include <QString>

#include "Token.h"
#include "AstNode.h"
#include "Function.h"

struct ParsePos
{
	int line = 0,
		pos = 0,
		lineOffset = 0;

    operator QString();
};

class ParseResult
{
public:
	ParseResult() = default;
	ParseResult(bool okay);
	explicit ParseResult(int status, ParsePos pos = {});
	ParseResult(int status, QString message, ParsePos pos = {});
	
	enum
	{
		NO_MATCH,
		COMPLETE,
		INCOMPLETE,
	};

	operator bool() const;

	ParsePos pos() const;
	QString message() const;
	int status() const;

private:
	int _status = COMPLETE;
	QString _message = "";
	ParsePos _pos;
};

class Parser
{
public:
    explicit Parser(QString input);

    QChar peek();
    QChar get();
    bool atEnd();

    void skip();

    QString line(int n) const;

    template <typename T>
    ParseResult parseSymbol(T *node);

    template <typename T>
    ParseResult parseIdentifier(T *node);

    template <typename T>
    ParseResult parseNumber(T *node);

    template <typename T>
    ParseResult parseVariable(T *node);

    template <typename T>
    ParseResult parseParens(T *node);

    ParseResult parseFunction(AstNode *node);

    template <typename T>
    ParseResult parseMany(QList<T> *list);

    template <typename T>
    ParseResult parseString(QList<T> *list);

    template <typename T>
    ParseResult parseOne(T *node);

    ParseResult parseSentence(Sentence *sentence);
    ParseResult parseFunctionDefinition(Function *function);

	ParsePos save() const;
	void reset(ParsePos pos);

private:
    int _pos = 0;
	int _line = 1;
	int _offset = 1;

    QString _input;
};

template <>
ParseResult Parser::parseOne<Token>(Token *node);

template <>
ParseResult Parser::parseOne<AstNode>(AstNode *node);
