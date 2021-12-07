#include <QCoreApplication>
#include <QDebug>

#include "Matcher.h"
#include "Token.h"
#include "AstNode.h"
#include "Parser.h"

int g_numFailed = 0;

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

void testParse(QString string)
{
    Parser parser{string};

    QList<AstNode> result = parser.parseMany();

    qDebug() << "\033[36mParse\033[0m" << string << result;
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

    testMatch("(a) = (a)", true,
                match({Token({Token('a')})}, {Token({Token('a')})}, VarContext()));
}

void testAllParses()
{
    testParse("all symbols");
    testParse("Identifier symbols Identifier");
    testParse("s.A");
    testParse("(s.A) Variable-quoted");
    testParse("<Func-name a b (c)>");
    testParse("<Prout hi>");
    testParse("(Prout hi)");
    testParse("(<Prout hi>)");
    testParse("<If T Then (<Prout hi>) Else (<Prout sorry>)>");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    testAllMatches();
    qDebug() << "";
    testAllParses();

    qDebug() << "";
    return testResults();
}
