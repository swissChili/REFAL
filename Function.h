#pragma once

#include "Token.h"
#include "AstNode.h"

class Sentence {
public:
	Sentence() = default;
	Sentence(QList<Token> pattern, QList<AstNode> result);

	QList<Token> pattern() const;
	QList<AstNode> result() const;

	operator QString() const;

protected:
	QList<Token> _pattern;
	QList<AstNode> _result;
};

class Function {
public:
	Function() = default;
	explicit Function(QString name);
	Function(QString name, QList<Sentence> sentences);

	void addSentence(Sentence sentence);

	QString name() const;
	QList<Sentence> sentences() const;

	operator QString() const;

private:
	QString _name;
	QList<Sentence> _sentences;
};
