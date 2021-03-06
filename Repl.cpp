#include <stdio.h>
#include <string.h>

#include <QDebug>

#include "Repl.h"
#include "Parser.h"
#include "PPrint.h"
#include "StdLib.h"

// JANK! librl isn't namespaced!
namespace ReadLine
{
#include <readline/readline.h>
#include <readline/history.h>
}


Repl::Repl()
{
	StdLib().load(_eval);
}

char *Repl::prompt()
{
	static char p[] = "\033[36mREFAL >\033[0m ";
	return p;
}

QString Repl::readLine()
{
	char *line = ReadLine::readline(prompt());

	if (!line)
	{
		_running = false;
		return "";
	}

	QString string = QString::fromUtf8(line);

	free(line);

	return string;
}

void Repl::addHistory(QString line)
{
	ReadLine::add_history(line.toUtf8());
}

void Repl::start()
{
	while (_running)
	{
		QString line = readLine().trimmed();

		QList<AstNode> expr;

		if (!line.isEmpty())
			addHistory(line);

		ParseResult ret;
        Parser parser{line};

		if (trySpecialCase(line))
		{}
        else if ((ret = tryEvaluate(parser, &expr)))
		{
			bool okay = true;
			QList<Token> out;

            for (const AstNode &node : qAsConst(expr))
			{
				RuntimeResult res = _eval.evaluate(node, VarContext());

				if (res.success())
				{
					out.append(res.result());
				}
				else
				{
					qDebug() << "Failed to evaluate" << node;
					qDebug() << res.message();
					okay = false;
					break;
				}
			}

			if (okay)
			{
                sout(pprint(out));
			}
		}
		else if (ret.status() == ParseResult::INCOMPLETE)
		{
            qDebug() << "Parse error: incomplete input:";
            sout(pprint(ret, parser));
            ReadLine::rl_insert_text("Hello there!");
            ReadLine::rl_redisplay();
		}
		else
		{
            qDebug() << "Parse error:" << ret.message();
		}
	}
}

ParseResult Repl::trySpecialCase(QString line)
{
	if (line.startsWith("."))
	{
		if (line == ".q" || line == ".quit")
		{
			_running = false;
		}
		else
		{
			qDebug().noquote() << "Unknown special command, try .help";
		}

		return true;
	}

	return false;
}

ParseResult Repl::tryEvaluate(Parser &parser, QList<AstNode> *expr)
{
    Function func;

	ParseResult ret;

    if ((ret = parser.parseFunctionDefinition(&func)))
	{
		_eval.addFunction(func);
		*expr = {};

		return true;
	}
	else if (ret.status() == ParseResult::INCOMPLETE)
	{
		return ret;
	}
	else
	{
		return parser.parseMany(expr);
	}
}
