#include "Parser.h"

#include <QDebug>

ParseResult::ParseResult(bool okay)
	: ParseResult((int)okay)
{}

ParseResult::ParseResult(int status, ParsePos pos)
	: ParseResult(status, "", pos)
{}

ParseResult::ParseResult(int status, QString message, ParsePos pos)
{
	_status = status;
    _message = message;
	_pos = pos;
}

ParseResult::operator bool() const
{
	return _status == COMPLETE;
}

ParsePos ParseResult::pos() const
{
	return _pos;
}

QString ParseResult::message() const
{
	return _message;
}

int ParseResult::status() const
{
	return _status;
}

Parser::Parser(QString input)
{
    _input = input;
}

QChar Parser::peek()
{
    if (atEnd())
        return 0;

    return _input[_pos];
}

QChar Parser::get()
{
    QChar c = _input[_pos++];

	if (c == '\n')
	{
		_line++;
		_offset = 0;
	}
	else
	{
		_offset++;
	}

	return c;
}

bool Parser::atEnd()
{
    return _pos >= _input.length();
}

void Parser::skip()
{
    while (peek().isSpace())
        get();
}

QString Parser::line(int n) const
{
    return _input.split("\n")[n];
}

ParsePos Parser::save() const
{
	return ParsePos{_line, _pos, _offset};
}

void Parser::reset(ParsePos pos)
{
	_line = pos.line;
	_pos = pos.pos;
	_offset = pos.lineOffset;
}

template <typename T>
ParseResult Parser::parseSymbol(T *node)
{
    skip();

    if (peek().isLetter())
    {
        *node = T(get());
        return true;
    }

    return false;
}

template <typename T>
ParseResult Parser::parseIdentifier(T *node)
{
    skip();

    QString buffer;

    if (peek().isUpper())
    {
        while (peek().isLetter() || peek() == '-' || peek() == '_' || peek().isNumber())
        {
            buffer += get();
        }

        *node = T(buffer);
        return true;
    }

    return false;
}

template <typename T>
ParseResult Parser::parseNumber(T *node)
{
    skip();

    QString buffer;

    if (peek().isDigit())
    {
        while (peek().isDigit())
            buffer += get();

        *node = T::fromInteger(buffer.toInt());
        return true;
    }

    return false;
}

template <typename T>
ParseResult Parser::parseVariable(T *node)
{
    skip();

	ParsePos pos = save();

    if (peek() == 's' || peek() == 'e' || peek() == 't')
    {
        char type = get().toLatin1();

        if (peek() == '.')
        {
            get();

            T nameNode;

            if (parseIdentifier(&nameNode))
            {
                *node = T(type, nameNode.name());
                return true;
            }
            else if (parseSymbol(&nameNode))
            {
                *node = T(type, QString(nameNode.symbol()));
                return true;
            }
			else
			{
				ParseResult ret(ParseResult::INCOMPLETE,
								"Expected identifier or symbol after . in variable",
                                save());

				reset(pos);

				return ret;
			}
        }
    }

	reset(pos);
    return false;
}

template <typename T>
ParseResult Parser::parseMany(QList<T> *list)
{
    QList<T> nodes;
    T next;
	ParseResult ret;

    while ((ret = parseOne(&next)))
    {
        nodes.append(next);
    }

	*list = nodes;

	if (ret.status() == ParseResult::INCOMPLETE)
		return ret;
	else
		return true;
}

template <typename T>
ParseResult Parser::parseParens(T *node)
{
    skip();

    ParsePos pos = save();

    if (peek() != '(')
        return false;

    get();

    QList<T> many;
	ParseResult ret = parseMany(&many);

    *node = T(many);

    skip();
    if (peek() != ')')
    {
		ret = ParseResult(ParseResult::INCOMPLETE, "Expected ) in parenthesized list", save());
		reset(pos);

		return ret;
    }

    get();

    return true;
}

ParseResult Parser::parseFunction(AstNode *node)
{
    skip();

    ParsePos pos = save();
	ParseResult ret;

    if (peek() != '<')
        return false;

    get();

    AstNode head;
    if (!(ret = parseIdentifier(&head)))
    {
        ParsePos endPos = save();
		reset(pos);
        return ParseResult(ParseResult::INCOMPLETE, "Expected identifier following < in function call", endPos);
    }

    QList<AstNode> body;
	ret = parseMany(&body);

	if (!ret)
	{
		reset(pos);
		return ret;
	}

    *node = AstNode(head.name(), body);

    skip();
    if (peek() != '>')
    {
		ret = ParseResult(ParseResult::INCOMPLETE, "Expected >", save());
		reset(pos);
        return ret;
    }

    get();

    return true;
}

template <>
ParseResult Parser::parseOne<Token>(Token *node)
{
	ParseResult ret;

	if ((ret = parseVariable(node)).status() == ParseResult::COMPLETE || ret.status() == ParseResult::INCOMPLETE)
		return ret;
	if ((ret = parseNumber(node)).status() == ParseResult::COMPLETE || ret.status() == ParseResult::INCOMPLETE)
		return ret;
	if ((ret = parseIdentifier(node)).status() == ParseResult::COMPLETE || ret.status() == ParseResult::INCOMPLETE)
		return ret;
	if ((ret = parseSymbol(node)).status() == ParseResult::COMPLETE || ret.status() == ParseResult::INCOMPLETE)
		return ret;
	return parseParens(node);
}

template <>
ParseResult Parser::parseOne<AstNode>(AstNode *node)
{
	ParseResult ret;

	if ((ret = parseFunction(node)).status() == ParseResult::COMPLETE || ret.status() == ParseResult::INCOMPLETE)
		return ret;
	if ((ret = parseVariable(node)).status() == ParseResult::COMPLETE || ret.status() == ParseResult::INCOMPLETE)
		return ret;
	if ((ret = parseNumber(node)).status() == ParseResult::COMPLETE || ret.status() == ParseResult::INCOMPLETE)
		return ret;
	if ((ret = parseIdentifier(node)).status() == ParseResult::COMPLETE || ret.status() == ParseResult::INCOMPLETE)
		return ret;
	if ((ret = parseSymbol(node)).status() == ParseResult::COMPLETE || ret.status() == ParseResult::INCOMPLETE)
		return ret;
	return parseParens(node);
}

ParseResult Parser::parseSentence(Sentence *sentence)
{
	ParsePos pos = save();

	if (peek() == '}')
	{
		return false;
	}

    QList<Token> pattern;
	ParseResult ret = parseMany(&pattern);

	if (!ret)
	{
		reset(pos);
		return ret;
	}

    skip();

    if (get() != '=')
    {
        ret = ParseResult(ParseResult::INCOMPLETE, "Expected = in sentence", save());
		reset(pos);
		return ret;
    }

    QList<AstNode> result;
	ret = parseMany(&result);

	if (!ret)
	{
		reset(pos);
		return ret;
	}

    skip();

    if (peek() != '}' && get() != ';')
    {
		ret = ParseResult(ParseResult::INCOMPLETE, "Expected ; or } after sentence", save());
		reset(pos);
		return ret;
    }

    *sentence = Sentence(pattern, result);

    return true;
}

ParseResult Parser::parseFunctionDefinition(Function *function)
{
	ParsePos pos = save();
	ParseResult ret;

    Token identifier;
    if (!(ret = parseIdentifier(&identifier)))
    {
		reset(pos);
        return ret;
    }

    QString name = identifier.name();
    Function func(name);

    skip();

    if (get() != '{')
    {
		reset(pos);
		return false;
    }

    Sentence sentence;
    while ((ret = parseSentence(&sentence)))
    {
        func.addSentence(sentence);
        skip();
    }

    if (ret.status() == ParseResult::INCOMPLETE)
	{
		reset(pos);
		return ret;
	}

    if (get() != '}')
    {
        ret = ParseResult(ParseResult::INCOMPLETE, "Expected } at end of function", save());
		reset(pos);
        return ret;
    }

    *function = func;
    return true;
}

ParsePos::operator QString()
{
    return QString::number(line) + ":" + QString::number(lineOffset);
}
