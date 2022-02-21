#include "StdLib.h"
#include "PPrint.h"

StdLib::StdLib()
{
    _print.addNativeSentence("e.Expr", [](VarContext args)
	{
        auto expr = args.expressionVar("Expr");
        sout(pprint(args.expressionVar("Expr")));
        return expr;
	});

    _prout.addNativeSentence("e.Expr", [](VarContext args)
	{
        auto expr = args.expressionVar("Expr");
        sout(pprint(expr));
		return QList<Token>();
	});
}

void StdLib::load(Evaluator &eval)
{
	eval.addFunction(_print);
	eval.addFunction(_prout);
}
