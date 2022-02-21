#pragma once

#include <QMap>
#include <QStack>

#include "Token.h"
#include "AstNode.h"
#include "Function.h"
#include "VarContext.h"

class RuntimeResult
{
public:
	RuntimeResult(QList<Token> result);
	RuntimeResult(QString message);
	
	bool success() const;
	QString message() const;
	QList<Token> result() const;

	operator QString() const;
	
private:
	bool _success = true;
	QString _errorMessage;
	QList<Token> _result;
};

class Evaluator {
public:
	Evaluator();

	void addFunction(Function func);
	void clearFunction(QString name);
	RuntimeResult evaluate(AstNode node, VarContext ctx);
	RuntimeResult callFunction(QString name, QList<Token> args);

private:
	QMap<QString, Function> _functions;
    QMap<QString, QStack<QList<Token>>> _vars;

protected:
    QList<Token> dig(QString name);
    QList<Token> copy(QString name);
    void bury(QString name, QList<Token> expression);
};

void rtError(QString brief, QString details);
