#include "Matcher.h"

#include <QDebug>

MatchResult match(QList<Token> data, QList<Token> pattern, VarContext context)
{
    if (data.empty() && pattern.empty())
    {
        return MatchResult{true, context};
    }

    if ((pattern.empty() && !data.empty()) || (data.empty() && !pattern.empty()))
    {
        return MatchResult{false, context};
    }

    Token ph = pattern.first();
    Token dataHead = data.first();
    pattern.removeFirst();

    if (ph.isSym() || ph.isIdent())
    {
        if (ph == data.first())
        {
            data.removeFirst();
            return match(data, pattern, context);
        }
        else
        {
            return MatchResult{false, context};
        }
    }
    else if (ph.isParen())
    {
        data.removeFirst();

        auto result = match(dataHead.parenContent(), ph.parenContent(), context);

        if (result.success)
        {
            return match(data, pattern, result.context);
        }
        else
        {
            return MatchResult{false, result.context};
        }
    }
    else if (ph.isVar())
    {
        // is var bound?
        if (context.exists(ph.name()))
        {
            // TODO: handle error elegantly if types don't match up (let's just assume the user isn't stupid)

            switch (ph.varType())
            {
            case 's':
            case 't':
                if (context.singleVar(ph.name()) == dataHead)
                {
                    data.removeFirst();
                    return match(data, pattern, context);
                }
                else
                {
                    return MatchResult{false, context};
                }
            case 'e':
                QList<Token> expected = context.expressionVar(ph.name());

                if (listStartsWith(data, expected))
                {
                    listDrop(data, expected.length());
                    return match(data, pattern, context);
                }
                else
                {
                    return MatchResult{false, context};
                }
            }
        }
        else
        {
            bool typeIsOk = false;

            switch (ph.varType())
            {
            case 't':
                if (dataHead.isParen())
                    typeIsOk = true;

            case 's':
                if (dataHead.isSym() || dataHead.isIdent())
                    typeIsOk = true;

                if (!typeIsOk)
                {
                    return MatchResult{false, context};
                }

                context.add(ph.varType(), ph.name(), dataHead);
                data.removeFirst();

                return match(data, pattern, context);

            case 'e':
                // Now this is tricky TODO: Optimize this to check if
                // there is an obvious length that this expression has
                // to be
                for (int matchSyms = 0; matchSyms <= data.length(); matchSyms++)
                {
                    QList<Token> slice = listSlice(data, 0, matchSyms);
                    VarContext newContext = context;
                    newContext.add(ph.varType(), ph.name(), slice);

                    MatchResult tryMatch = match(
						listSlice(data, matchSyms, data.length()),
						pattern, newContext);

                    if (tryMatch.success)
                    {
                        return tryMatch;
                    }
                }
                // If this worked we would have returned already
                return MatchResult{false, context};

            default:
                qDebug() << "#TYPE_ERROR";
                return MatchResult{false, context};
            }
        }
    }

    qFatal("FALLING THROUGH, THIS SHOULD NOT HAPPEN");
    // Fallthrough
    return MatchResult{false, context};
}
