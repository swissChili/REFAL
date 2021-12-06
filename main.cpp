#include <QCoreApplication>
#include <QDebug>

#include "Matcher.h"
#include "Token.h"

int g_numFailed = 0;

void printResult(const QString &test, bool shouldBe, const MatchResult &result) {
    if (result.success != shouldBe) {
        g_numFailed++;
        qDebug() << "\nTEST FAILS:";
        qDebug() << "with context" << result.context;
    }

    qDebug() << "MatchResult" << test << result.success;

    if (result.success != shouldBe) {
        qDebug() << "";
    }
}

int testResults() {
    if (g_numFailed == 0) {
        qDebug() << "ALL TESTS SUCCEEDED";
    } else {
        qDebug() << g_numFailed << "TESTS FAILED";
    }

    return g_numFailed;
}

void runTests() {
    printResult("a = a", true, match({Token('a')}, {Token('a')}, VarContext()));

    printResult("s.a = y", true, match({Token('y')}, {Token('s', "a")}, VarContext()));

    LTok sameTwo = {Token('s', "a"), Token('s', "a")};
    printResult("s.a s.a = aa", true, match({Token('a'), Token('a')}, sameTwo, VarContext()));
    printResult("s.a s.a = ab", false, match({Token('a'), Token('b')}, sameTwo, VarContext()));

    LTok sameStartEnd = {
            Token('s', "a"),
            Token('e', "middle"),
            Token('s', "a")
    };
    printResult("s.a e.middle s.a = aea", true,
                match({
                              Token('a'), Token('e'), Token('a')
                      }, sameStartEnd, VarContext()));

    printResult("s.a e.middle s.a = aef Hi a", true,
                match({
                              Token('a'), Token('e'), Token('f'), Token("Hi"), Token('a')
                      }, sameStartEnd, VarContext()));

    printResult("s.a e.middle s.a = aef Hi c", false,
                match({
                              Token('a'), Token('e'), Token('f'), Token("Hi"), Token('c')
                      }, sameStartEnd, VarContext()));

    LTok parenthesized = {
            Token(LTok({
                               Token('s', "a")
                       })),
            Token('e', "Middle"),
            Token('s', "a"),
    };
    LTok parenTest1 = {
            Token(LTok({
                               Token('y')
                       })),
            Token('f'),
            Token("MiddleStuff"),
            Token('y')
    };

    printResult("(s.a) e.Middle s.a = (y)f MiddleStuff y", true,
                match(parenTest1, parenthesized, VarContext()));

    printResult("(a) = (a)", true,
                match({Token({Token('a')})}, {Token({Token('a')})}, VarContext()));
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    runTests();

    return testResults();
}
