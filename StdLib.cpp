#include "StdLib.h"
#include "PPrint.h"

StdLib::StdLib()
{
	_print.addNativeSentence("e.Expr", [](QList<Token> args)
	{
		sout(pprint(args));
		return args;
	});

	_prout.addNativeSentence("e.Expr", [](QList<Token> args)
	{
		sout(pprint(std::move(args)));
		return QList<Token>();
	});
}

void StdLib::load(Evaluator &eval)
{
	eval.addFunction(_print);
	eval.addFunction(_prout);
}
