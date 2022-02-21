#include "Function.h"
#include "Parser.h"
#include <QDebug>

template <typename T>
QString join(QList<T> list, QString sep)
{
	QStringList strings;

	for (const T &item : list)
	{
		strings.append(static_cast<QString>(item));
	}

	return strings.join(sep);
}

Sentence::~Sentence()
{
}

Sentence::Sentence(QList<Token> pattern, QList<AstNode> result)
{
	_pattern = pattern;
	_result = result;
}

Sentence::Sentence(QList<Token> pattern, SentenceResultFn result)
{
	_pattern = pattern;
	_native = result;
}

bool Sentence::isExternal() const
{
	return _native != nullptr;
}

QList<Token> Sentence::externResult(MatchResult args) const
{
    return _native(args.context);
}

QList<Token> Sentence::pattern() const
{
	return _pattern;
}

QList<AstNode> Sentence::result() const
{
	return _result;
}

Sentence::operator QString() const
{
	return join(_pattern, " ") + " = " + join(_result, " ") + ";";
}

Function::Function(QString name)
	: Function(name, {})
{
}

Function::Function(QString name, QList<Sentence> sentences)
{
	_name = name;
	_sentences = sentences;
}

void Function::addSentence(Sentence sentence)
{
	_sentences.append(sentence);
}

void Function::addNativeSentence(QString pattern, SentenceResultFn fn)
{
	Parser parser(std::move(pattern));
	QList<Token> parsedPattern;

	ParseResult res = parser.parseMany(&parsedPattern);
	if (!res)
	{
		qDebug() << "Failed to parse pattern for native sentence";
		qDebug() << res.message();
	}
	else
	{
		addSentence(Sentence(parsedPattern, std::move(fn)));
	}
}

QString Function::name() const
{
	return _name;
}

QList<Sentence> Function::sentences() const
{
	return _sentences;
}

Function::operator QString() const
{
	QString buffer = name() + " { ";
	int leftPadding = buffer.length();

	QString spaces;
	for (int i = 0; i < leftPadding; i++)
		spaces += " ";

	for (int i = 0; i < _sentences.length(); i++)
	{
		if (i)
			buffer += "\n" + spaces;

		buffer += static_cast<QString>(_sentences[i]);
	}

	buffer += " }";

	return buffer;
}
