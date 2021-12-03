#include "Matcher.h"

MatchResult match(QList<Token> data, QList<Token> pattern, VarContext context) {
    if (data.empty() && pattern.empty()) {
        return MatchResult{true, context};
    }

    Token patternHead = pattern.first();
    Token dataHead = data.first();
    pattern.removeFirst();

    if (patternHead.isSym() || patternHead.isIdent()) {
        if (patternHead == pattern.first()) {
            data.removeFirst();
            return match(data, pattern, context);
        } else {
            return MatchResult{false, context};
        }
    } else if (patternHead.isParen() && dataHead.isParen()) {
        data.removeFirst();
        auto result = match(dataHead.parenContent(), patternHead.parenContent(), context);

        if (result.success) {
            return match(data, pattern, result.context);
        } else {
            return MatchResult{false, result.context};
        }
    }
}
