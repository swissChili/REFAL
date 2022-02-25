#include "StdLib.h"
#include "PPrint.h"

#include <QEventLoop>
#include <QTimer>

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

    _sleep.addNativeSentence("s.Time", [](VarContext args)
    {
        Token time = args.singleVar("Time");

        if (time.type() != Token::INTEGER)
            rtError("Invalid argument", "First argument to <Time> must be an integer, got " + pprint(time));

        QEventLoop loop;
        QTimer timer;
        timer.connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        timer.start(time.integer());
        loop.exec();

        return QList<Token>();
    });
}

void StdLib::load(Evaluator &eval)
{
	eval.addFunction(_print);
	eval.addFunction(_prout);
    eval.addFunction(_sleep);
}
