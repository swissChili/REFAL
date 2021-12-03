#pragma once

#include "Token.h"
#include "VarContext.h"

struct MatchResult {
    bool success;
    VarContext context;
};

MatchResult match(QList<Token> data, QList<Token> pattern, VarContext context);
