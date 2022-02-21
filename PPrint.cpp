#include "PPrint.h"

#include <QTextStream>

void sout(QString string)
{
    QTextStream(stdout) << string << '\n';
}

QString pprint(ParseResult val, const Parser &parser)
{
    if (val)
    {
        return "\033[32mOK\033[0m";
    }

    QString highlighted = parser.line(val.pos().line - 1) + "\n";

    for (int i = 1; i < val.pos().lineOffset; i++)
    {
        highlighted += " ";
    }
    highlighted += "\033[31m^~~\033[0m";

    return val.message() + " at " + val.pos() + "\n" + highlighted;
}

void eout(QString string)
{
    QTextStream(stderr) << string << '\n';
}
