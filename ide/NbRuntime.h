#pragma once

#include <QObject>
#include <QThread>
#include <QQueue>

#include "Cell.h"
#include "../Token.h"
#include "../Evaluator.h"
#include "../Parser.h"

class NbRuntime : public QObject
{
    Q_OBJECT

public:
    explicit NbRuntime(QObject *parent = nullptr);

    void reset();

public slots:
    void queueCell(Cell *cell);
    void unqueueCell(Cell *cell);

signals:
    void cellFinishedRunning(Cell *cell, RuntimeResult result);
    void cellFailedToParse(Cell *cell, ParseResult result, Parser parser);
    void cellWaiting(Cell *cell);
    void cellRunning(Cell *cell);
    void cellQuit(Cell *cell);

protected:
    void evalRemaining();

    Evaluator _eval;
    QQueue<Cell *> _cells;
    Cell *_running = nullptr;
    VarContext _ctx;
};
