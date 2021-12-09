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

	ReadLine::add_history(line);

	QString string = QString::fromUtf8(line);

	free(line);

	return string;
}

void Repl::start()
{
	while (_running)
	{
		QString line = readLine();

		line = line.trimmed();

		QList<AstNode> expr;

		if (trySpecialCase(line))
		{}
		else if (tryEvaluate(line, &expr))
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
				qDebug() << pprint(out);
			}
		}
		else
		{
			qDebug() << "What?";
		}
	}
}

bool Repl::trySpecialCase(QString line)
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

bool Repl::tryEvaluate(QString line, QList<AstNode> *expr)
{
	Parser parser(line);
	Function func;

	if (parser.parseFunctionDefinition(&func))
	{
		_eval.addFunction(func);
		*expr = {};

		return true;
	}

	*expr = parser.parseMany<AstNode>();
	parser.skip();

	return parser.atEnd();
}
