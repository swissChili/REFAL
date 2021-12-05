#include <QCoreApplication>
#include <QDebug>

#include "Matcher.h"
#include "Token.h"

int g_numFailed = 0;

void printResult(const QString &test, bool shouldBe, const MatchResult &result) {
    if (result.success != shouldBe) {
        g_numFailed++;
        qDebug() << "TEST FAILS:";
    }

    qDebug() << "MatchResult" << test << result.success;
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
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    runTests();

    return testResults();
}
