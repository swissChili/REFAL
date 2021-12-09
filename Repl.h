#pragma once

#include <QString>
#include <QList>

#include "Evaluator.h"
#include "AstNode.h"

class Repl
{
public:
	Repl();

	void start();
	char *prompt();

protected:
	QString readLine();
	bool trySpecialCase(QString line);
	bool tryEvaluate(QString line, QList<AstNode> *expr);

	Evaluator _eval;

	bool _running = true;
};
