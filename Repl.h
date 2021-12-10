#pragma once

#include <QString>
#include <QList>

#include "Evaluator.h"
#include "AstNode.h"
#include "Parser.h"

class Repl
{
public:
	Repl();

	void start();
	char *prompt();

protected:
	QString readLine();
	void addHistory(QString line);
	ParseResult trySpecialCase(QString line);
	ParseResult tryEvaluate(QString line, QList<AstNode> *expr);

	Evaluator _eval;

	bool _running = true;
};
