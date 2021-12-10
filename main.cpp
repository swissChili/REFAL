#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>

#include "Matcher.h"
#include "Token.h"
#include "AstNode.h"
#include "Parser.h"
#include "Evaluator.h"
#include "VarContext.h"
#include "Repl.h"
#include "PPrint.h"

int g_numFailed = 0;


void testEval(QString function, QString expression, QString expected)
{
	Evaluator eval;
	Parser funcParser(function),
		exprParser(expression),
		resParser(expected);

	Function func;

	QList<AstNode> expr;
	ParseResult exprRet = exprParser.parseMany<AstNode>(&expr);
	QList<Token> res;
	ParseResult resRet = resParser.parseMany<Token>(&res);
	ParseResult funcRet;

	QList<Token> result;

	exprParser.skip();
	resParser.skip();
	while ((funcRet = funcParser.parseFunctionDefinition(&func)))
	{
		eval.addFunction(func);
	}

	funcParser.skip();

	if (!exprRet || !resRet || funcRet.status() == ParseResult::INCOMPLETE)
	{
        g_numFailed++;
        qDebug() << "\n\033[31mTEST FAILS:\033[0m";
		qDebug() << "Failed to fully parse expression, function or result";
		qDebug() << function << expression << expected;
		qDebug() << funcRet.message() << exprRet.message() << resRet.message();
		qDebug() << funcRet << exprRet << resRet;

		goto end;
	}

	for (const AstNode &node : expr)
	{
		RuntimeResult rr = eval.evaluate(node, VarContext());

		if (!rr.success())
		{
			g_numFailed++;
			qDebug() << "\n\033[31mTEST FAILS:\033[0m";
			qDebug() << "Runtime error while evaluating" << node;
			qDebug() << rr;

			goto end;
		}

		result.append(rr.result());
	}

	if (result != res)
	{
		g_numFailed++;
		qDebug() << "\n\033[31mTEST FAILS:\033[0m";
		qDebug() << "Expected result" << res;
		qDebug() << "Got" << result;
	}

end:
	qDebug() << "\033[36mEvaluate\033[0m" << function << expression << "->" << result;
}

void testEval(QString function, QString expression, QList<Token> expected)
{
	testEval(function, expression, pprint(expected));
}

void testMatch(const QString &test, bool shouldBe, const MatchResult &result)
{
    if (result.success != shouldBe)
    {
        g_numFailed++;
        qDebug() << "\n\033[31mTEST FAILS:\033[0m";
        qDebug() << "with context" << result.context;
    }

    qDebug() << "\033[36mMatchResult\033[0m" << test << result.success;

    if (result.success != shouldBe)
    {
        qDebug() << "";
    }
}

void testMatch(QString data, QString pattern, bool shouldBe = true)
{
    Parser dataParser(data),
        patternParser(pattern);

	QList<Token> parsedData, parsedPattern;
	
	dataParser.parseMany<Token>(&parsedData);
	patternParser.parseMany<Token>(&parsedPattern);

    testMatch(pattern + " = " + data, shouldBe,
			  match(parsedData, parsedPattern, VarContext()));
}

void testParseAst(QString string, QList<AstNode> equals = {})
{
    Parser parser{string};

    QList<AstNode> result;
	parser.parseMany<AstNode>(&result);

	if (!equals.empty() && result != equals)
	{
        g_numFailed++;
        qDebug() << "\n\033[31mTEST FAILS:\033[0m";
        qDebug() << "Expected" << pprint(equals);
	}

    qDebug() << "\033[36mParse\033[0m" << string << pprint(result);
}

void testParseFunc(QString string)
{
    Parser parser{string};
	ParseResult ret;

    Function func;

    if (!(ret = parser.parseFunctionDefinition(&func)))
    {
        g_numFailed++;
        qDebug() << "\n\033[31mTEST FAILS:\033[0m";
		qDebug() << ret.message();
        qDebug() << string;
    }
    else
    {
        qDebug() << "\033[36mFunction\033[0m";
        qDebug().noquote() << func;
    }
}

int testResults()
{
    if (g_numFailed == 0)
    {
        qDebug() << "\033[32mALL TESTS SUCCEEDED\033[0m";
    }
    else
    {
        qDebug().nospace() << "\033[31m" << g_numFailed << " TESTS FAILED\033[0m";
    }

    return g_numFailed;
}

void testAllMatches()
{
    testMatch("a = a", true, match({Token('a')}, {Token('a')}, VarContext()));

    testMatch("s.a = y", true, match({Token('y')}, {Token('s', "a")}, VarContext()));

    LTok sameTwo = {Token('s', "a"), Token('s', "a")};
    testMatch("s.a s.a = aa", true, match({Token('a'), Token('a')}, sameTwo, VarContext()));
    testMatch("s.a s.a = ab", false, match({Token('a'), Token('b')}, sameTwo, VarContext()));

    LTok sameStartEnd = {
        Token('s', "a"),
        Token('e', "middle"),
        Token('s', "a")};
    testMatch("s.a e.middle s.a = aea", true,
                match({Token('a'), Token('e'), Token('a')}, sameStartEnd, VarContext()));

    testMatch("s.a e.middle s.a = aef Hi a", true,
                match({Token('a'), Token('e'), Token('f'), Token("Hi"), Token('a')}, sameStartEnd, VarContext()));

    testMatch("s.a e.middle s.a = aef Hi c", false,
                match({Token('a'), Token('e'), Token('f'), Token("Hi"), Token('c')}, sameStartEnd, VarContext()));

    LTok parenthesized = {
        Token(LTok({Token('s', "a")})),
        Token('e', "Middle"),
        Token('s', "a"),
    };
    LTok parenTest1 = {
        Token(LTok({Token('y')})),
        Token('f'),
        Token("MiddleStuff"),
        Token('y')};

    testMatch("(s.a) e.Middle s.a = (y)f MiddleStuff y", true,
                match(parenTest1, parenthesized, VarContext()));
    // testMatch("(y)f Middle-stuff y", "(s.a) e.Middle s.a");

    testMatch("(a)", "(a)");
	testMatch("hello", "s.A e.Rest");
	testMatch("123", "123");
	testMatch("(123)", "t.a");
	testMatch("(123)", "(s.a)");
}

void testAllParses()
{
    testParseAst("all symbols");
    testParseAst("Identifier symbols Identifier");
    testParseAst("s.A");
    testParseAst("(s.A) Variable-quoted");
    testParseAst("<Func-name a b (c)>");
    testParseAst("<Prout hi>");
    testParseAst("(Prout hi)");
    testParseAst("(<Prout hi>)");
    testParseAst("<If T Then (<Prout hi>) Else (<Prout sorry>)>");
    testParseAst("(s.a) e.Middle s.a");
    testParseAst("Hello; Goodbye");
    testParseAst("Key = Value");
	testParseAst("123", {AstNode("123", 10)});
	testParseAst("12 00", {AstNode::fromInteger(12), AstNode::fromInteger(0)});
}

void testAllFunctionDefs()
{
    testParseFunc("Test { = HI; }");
    testParseFunc("Palindrome { = T; s.A = T; s.A e.Middle s.A = <Palindrome e.Middle>; e.Ignored = F; } ");
}

void testAllEvals()
{
	testEval("First {s.A e.Rest = s.A;}", "<First hello>", "h");
	testEval("Number { = 123; }", "<Number>", QList<Token>{Token::fromInteger(123)});
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	QCoreApplication::setApplicationName("REFAL");
	QCoreApplication::setApplicationVersion("1.0-a1");

	QCommandLineParser cli;
	cli.setApplicationDescription("REFAL interpreter");
	cli.addHelpOption();
	cli.addVersionOption();

	QCommandLineOption testOption(QStringList{"t", "test"}, "Run test suite.");
	cli.addOption(testOption);

	cli.process(a);

	if (cli.isSet(testOption))
	{
		testAllMatches();
		qDebug() << "";
		testAllParses();
		qDebug() << "";
		testAllFunctionDefs();
		qDebug() << "";
		testAllEvals();

		qDebug() << "";

		return testResults();
	}
	else
	{
		Repl repl;
		repl.start();
	}
}
