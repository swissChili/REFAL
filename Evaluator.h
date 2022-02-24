#pragma once

#include <QMap>
#include <QStack>
#include <QException>

#include "Token.h"
#include "AstNode.h"
#include "Function.h"
#include "VarContext.h"

class RuntimeResult
{
public:
    RuntimeResult(const RuntimeResult &other) = default;
    RuntimeResult(QList<Token> result = {});
	RuntimeResult(QString message);
	
    RuntimeResult &operator =(const RuntimeResult &other);
    RuntimeResult operator +(const RuntimeResult &other);
    RuntimeResult &operator +=(const RuntimeResult &other);

	bool success() const;
	QString message() const;
	QList<Token> result() const;

	operator QString() const;
	
private:
	bool _success = true;
	QString _errorMessage;
	QList<Token> _result;
};

class EvalQuitException : public QException
{
public:
    EvalQuitException() = default;
    EvalQuitException(const EvalQuitException &other) = default;

    void raise() const override;
    EvalQuitException *clone() const override;
};

class StackOverflowException : public QException
{
public:
    StackOverflowException(AstNode failedAt = AstNode());
    StackOverflowException(const StackOverflowException &other) = default;

    AstNode failedAt() const;

    void raise() const override;
    StackOverflowException *clone() const override;

    operator QString() const;

private:
    AstNode _failedAt;
};

class Evaluator {
public:
	Evaluator();

	void addFunction(Function func);
	void clearFunction(QString name);
    RuntimeResult evaluate(AstNode node, VarContext ctx, int recursionDepth = 0);
    RuntimeResult callFunction(QString name, QList<Token> args, int recursionDepth);

    // Throws an EvalQuitException
    void quit();

private:
	QMap<QString, Function> _functions;
    QMap<QString, QStack<QList<Token>>> _vars;
    bool _shouldContinue = true;
    int _recursionLimit = 1024;

protected:
    QList<Token> dig(QString name);
    QList<Token> copy(QString name);
    void bury(QString name, QList<Token> expression);
};

void rtError(QString brief, QString details);
