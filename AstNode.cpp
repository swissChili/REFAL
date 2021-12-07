#include "AstNode.h"

AstNode::AstNode(QString function, QList<AstNode> args)
{
    _type = FUNC;
    _stringVal = function;
    _listVal = args;
}

QList<AstNode> AstNode::funcArgs()
{
    return _listVal;
}

bool AstNode::isFunc() const
{
    return _type == FUNC;
}

AstNode::operator QString() const
{
    if (isFunc())
    {
        QStringList args;

        for (const AstNode &node : _listVal)
        {
            args.append(static_cast<QString>(node));
        }

        return "<" + _stringVal + " " + args.join(" ") + ">";
    }

    return TokenBase<AstNode>::operator QString();
}
