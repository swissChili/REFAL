#pragma once

#include "Token.h"
#include "AstNode.h"
#include "VarContext.h"
#include "Matcher.h"

using SentenceResultFn = std::function<QList<Token> (VarContext)>;

class Sentence {
public:
	Sentence() = default;
	~Sentence();
	Sentence(QList<Token> pattern, QList<AstNode> result);
	Sentence(QList<Token> pattern, SentenceResultFn result);

	bool isExternal() const;
    QList<Token> externResult(MatchResult args) const;

	QList<Token> pattern() const;
	QList<AstNode> result() const;

	operator QString() const;

protected:
	QList<Token> _pattern;
	QList<AstNode> _result;
	SentenceResultFn _native = nullptr;
};

class Function {
public:
	Function() = default;
	explicit Function(QString name);
	Function(QString name, QList<Sentence> sentences);

	void addSentence(Sentence sentence);
	void addNativeSentence(QString pattern, SentenceResultFn fn);

	QString name() const;
	QList<Sentence> sentences() const;

	operator QString() const;

private:
	QString _name;
	QList<Sentence> _sentences;
};
