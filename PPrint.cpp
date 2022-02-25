#include "PPrint.h"

#include <QTextStream>

void sout(QString string)
{
    QTextStream(stdout) << string << '\n';
}

QString pprint(ParseResult val, const Parser &parser, PPrint::Style style)
{
    if (val)
    {
        return "\033[32mOK\033[0m";
    }

    QString newline = (style == PPrint::ANSI ? "\n" : "<br>");

    QString highlighted = parser.line(val.pos().line - 1) + newline;

    for (int i = 1; i < val.pos().lineOffset; i++)
    {
        if (style == PPrint::ANSI)
            highlighted += " ";
        else
            highlighted += "&nbsp;";
    }

    if (style == PPrint::ANSI)
    {
        highlighted += "\033[31m^~~\033[0m";
    }
    else if (style == PPrint::HTML)
    {
        highlighted += "<font color=\"red\">^~~</font>";
    }

    return val.message() + " at " + val.pos() + newline + highlighted;
}

void eout(QString string)
{
    QTextStream(stderr) << string << '\n';
}

QString pprint(QList<Token> val)
{
    return pprintDense(val);
}

QString pprint(QList<AstNode> val)
{
    return pprintDense(val);
}
