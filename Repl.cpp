#include <stdio.h>
#include <string.h>

#include <QDebug>

#include "Repl.h"
#include "Parser.h"
#include "PPrint.h"

// JANK! librl isn't namespaced!
namespace ReadLine
{
#include <readline/readline.h>
#include <readline/history.h>
}


Repl::Repl()
{
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

		if (trySpecialCase(line))
		{}
		else if ((ret = tryEvaluate(line, &expr)))
		{
			bool okay = true;
			QList<Token> out;

			for (const AstNode &node : expr)
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
				qDebug().noquote() << pprint(out);
			}
		}
		else if (ret.status() == ParseResult::INCOMPLETE)
		{
			qDebug() << "INCOMPLETE";
			ReadLine::rl_line_buffer = line.toUtf8().data();
			// ReadLine::rl_insert_text("\n");
			// ReadLine::rl_on_new_line();
			ReadLine::rl_redisplay();
			// ReadLine::rl_message("Incomplete");
		}
		else
		{
			qDebug() << "What?";
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

ParseResult Repl::tryEvaluate(QString line, QList<AstNode> *expr)
{
	Parser parser(line);
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
