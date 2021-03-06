#pragma once

#include "Token.h"

class AstNode : public TokenBase<AstNode>
{
public:
    using TokenBase<AstNode>::TokenBase;

    AstNode(QString function, QList<AstNode> args);

    enum
    {
        FUNC = TOKEN_TYPE_LAST
    };

    QList<AstNode> funcArgs();

    bool isFunc() const;

    operator QString() const;
};
