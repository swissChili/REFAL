#include "Evaluator.h"
#include "Function.h"
#include "Matcher.h"
#include "VarContext.h"
#include "PPrint.h"

#include <QDebug>

RuntimeResult::RuntimeResult(QList<Token> result)
{
	_result = result;
}

RuntimeResult::RuntimeResult(QString message)
{
	_errorMessage = message;
	_success = false;
}

bool RuntimeResult::success() const
{
	return _success;
}

QString RuntimeResult::message() const
{
	return _errorMessage;
}

QList<Token> RuntimeResult::result() const
{
	return _result;
}

RuntimeResult::operator QString() const
{
	return QString(_success) + " " + _errorMessage;
}

Evaluator::Evaluator()
{
    Function buryFn("Br");
    buryFn.addNativeSentence("s.Name '=' e.Expr", [this](VarContext args)
    {
        Token name = args.singleVar("Name");
        if (name.type() != Token::IDENT)
            rtError("Invalid argument", "First argument to <Br> must be an identifier, received " + pprint(name));

        bury(name.name(), args.expressionVar("Expr"));

        return QList<Token>();
    });
    addFunction(buryFn);

    Function digFn("Dg");
    digFn.addNativeSentence("s.Name", [this](VarContext args)
    {
        Token name = args.singleVar("Name");
        if (name.type() != Token::IDENT)
            rtError("Invalid argument", "First argument to <Dg> must be an identifier, received " + pprint(name));

        return dig(name.name());
    });
    addFunction(digFn);

    Function copyFn("Cp");
    copyFn.addNativeSentence("s.Name", [this](VarContext args)
    {
        Token name = args.singleVar("Name");
        if (name.type() != Token::IDENT)
            rtError("Invalid argument", "First argument to <Cp> must be an identifier, received " + pprint(name));

        return copy(name.name());
    });
    addFunction(copyFn);
}

void Evaluator::addFunction(Function func)
{
	_functions[func.name()] = func;
}

void Evaluator::clearFunction(QString name)
{
	_functions.remove(name);
}

RuntimeResult Evaluator::evaluate(AstNode node, VarContext ctx)
{
	if (node.isSym())
	{
		return RuntimeResult(QList<Token>{Token(node.symbol())});
	}
	else if (node.isIdent())
	{
		return RuntimeResult(QList<Token>{Token(node.name())});
	}
	else if (node.isInteger())
	{
		return RuntimeResult(QList<Token>{Token::fromInteger(node.integer())});
	}
	else if (node.isVar())
	{
		if (!ctx.exists(node.name()) || ctx.exists(node.name()) != node.symbol())
			return RuntimeResult("Variable " + node + " is not defined");

		if (node.symbol() == 'e')
		{
			return RuntimeResult(ctx.expressionVar(node.name()));
		}
		else
		{
			return RuntimeResult(QList<Token>{
					ctx.singleVar(node.name())
				});
		}
	}
	else if (node.isParen())
	{
		QList<Token> result;

		for (const AstNode &n : node.parenContent())
		{
			RuntimeResult internalResult = evaluate(n, ctx);
			if (!internalResult.success())
				return internalResult;

			result.append(internalResult.result());
		}

		return RuntimeResult(QList<Token>{
				Token(result)
			});
	}
	else if (node.isFunc())
	{
		QList<Token> args;

		for (const AstNode &arg : node.funcArgs())
		{
			RuntimeResult internalResult = evaluate(arg, ctx);
			if (!internalResult.success())
				return internalResult;

			args.append(internalResult.result());
		}

		return callFunction(node.name(), args);
	}

	return RuntimeResult("#TYPE_ERROR");
}

RuntimeResult Evaluator::callFunction(QString name, QList<Token> args)
{
	if (!_functions.contains(name))
		return RuntimeResult("Function " + name + " is not defined.");

	Function func = _functions[name];

	for (const Sentence &sentence : func.sentences())
	{
		MatchResult res = match(args, sentence.pattern(), VarContext());

		if (!res.success)
			continue;

		if (sentence.isExternal())
		{
            return RuntimeResult(sentence.externResult(res));
		}

		QList<Token> final;
		for (const AstNode &node : sentence.result())
		{
			RuntimeResult argRes = evaluate(node, res.context);
			if (!argRes.success())
				return argRes;

			final.append(argRes.result());
		}

		return RuntimeResult(final);
	}

    return RuntimeResult("Function " + name + " had no matching sentences for input");
}

QList<Token> Evaluator::dig(QString name)
{
    if (_vars.contains(name))
    {
        QList<Token> value = _vars[name].pop();

        if (_vars[name].empty())
        {
            _vars.remove(name);
        }

        return value;
    }
    else
    {
        return {};
    }
}

QList<Token> Evaluator::copy(QString name)
{
    if (_vars.contains(name))
    {
        return _vars[name].last();
    }
    else
    {
        return {};
    }
}

void Evaluator::bury(QString name, QList<Token> expression)
{
    if (!_vars.contains(name))
    {
        _vars[name] = QStack<QList<Token>>();
    }

    _vars[name].push(expression);
}

void rtError(QString brief, QString details)
{
    eout("Runtime Error: " + brief);
    eout(details);
}
