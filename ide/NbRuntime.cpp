#include <QCoreApplication>

#include "NbRuntime.h"
#include "../Parser.h"
#include "../StdLib.h"

NbRuntime::NbRuntime(QObject *parent)
    : QObject(parent)
{
    StdLib().load(_eval);
}

void NbRuntime::reset()
{
    _eval.reset();
    _ctx = {};
}

void NbRuntime::queueCell(Cell *cell)
{
    if (!_cells.contains(cell))
    {
        qInfo() << "Queueing cell";

        _cells.append(cell);

        emit cellWaiting(cell);

        if (!_running)
            evalRemaining();
    }
}

void NbRuntime::unqueueCell(Cell *cell)
{
    if (cell == _running)
    {
        // Exception should propagate back up to evalRemaining()
        _eval.quit();
    }
    else
    {
        _cells.removeOne(cell);
    }
}

void NbRuntime::evalRemaining()
{
    qInfo() << "evalRemaining";

    while (!_cells.empty())
    {
        QCoreApplication::processEvents();

        Cell *cell = _cells.first();
        _cells.removeFirst();

        _running = cell;

        Parser parser(cell->code());
        RuntimeResult result;

        emit cellRunning(cell);

        try
        {
            // Allow this cell to be quit
            QCoreApplication::processEvents();

            while (true)
            {
                ParseResult ret;
                Function func;
                QList<AstNode> ast;

                if ((ret = parser.parseFunctionDefinition(&func)))
                {
                    _eval.addFunction(func);
                }
                else if (ret.status() == ParseResult::INCOMPLETE)
                {
                    emit cellFailedToParse(cell, ret, parser);
                    goto endOfCell; // JANK!
                }
                else if ((ret = parser.parseMany(&ast)) && !ast.empty())
                {
                    for (const AstNode &node : ast)
                    {
                        RuntimeResult nodeRes = _eval.evaluate(node, _ctx);
                        result += nodeRes;

                        if (!nodeRes.success())
                        {
                            goto endOfParseLoop;
                        }
                    }
                }
                else if (ret.status() == ParseResult::INCOMPLETE)
                {
                    emit cellFailedToParse(cell, ret, parser);
                    break;
                }
                else
                {
                    parser.skip();

                    if (!parser.atEnd())
                    {
                        emit cellFailedToParse(cell, ParseResult(ParseResult::NO_MATCH, "Garbage at end of input", parser.save()), parser);
                        goto endOfCell;
                    }

                    break;
                }
            }

        endOfParseLoop:
            emit cellFinishedRunning(cell, result);

        endOfCell:
            _running = nullptr;
        }
        catch (EvalQuitException &ex)
        {
            _running = nullptr;
            emit cellQuit(cell);
        }
        catch (StackOverflowException &ex)
        {
            _running = nullptr;
            emit cellFinishedRunning(cell, RuntimeResult(ex));
        }
        catch (AssertionException &ex)
        {
            _running = nullptr;
            emit cellFinishedRunning(cell, RuntimeResult(ex));
        }
    }
}
