#include "Notebook.h"
#include "CellModel.h"
#include "../PPrint.h"

Notebook::Notebook(QObject *parent)
    : QObject(parent)
    , _cellModel(new CellModel(this))
{
    connect(&_rt, &NbRuntime::cellFailedToParse, this, &Notebook::cellFailedToParse);
    connect(&_rt, &NbRuntime::cellFinishedRunning, this, &Notebook::cellFinishedRunning);
    connect(&_rt, &NbRuntime::cellQuit, this, &Notebook::cellQuit);
    connect(&_rt, &NbRuntime::cellRunning, this, &Notebook::cellRunning);
    connect(&_rt, &NbRuntime::cellWaiting, this, &Notebook::cellWaiting);

    _rt.start();
}

Notebook::Notebook(const Notebook &other, QObject *parent)
    : Notebook(parent)
{
    for (const Cell *cell : other._cells)
    {
        _cells.append(new Cell(*cell, this));
    }
}

CellModel *Notebook::cellModel()
{
    return _cellModel;
}

void Notebook::runCell(QUuid uuid)
{
    qInfo() << "Running cell" << uuid;
    _rt.queueCell(Cell::cellFromUuid(uuid));
}

void Notebook::quitCell(QUuid uuid)
{
    _rt.unqueueCell(Cell::cellFromUuid(uuid));
}

void Notebook::cellFinishedRunning(Cell *cell, RuntimeResult result)
{
    qInfo() << "cellFinishedRunning" << cell->uuid() << pprint(result);
    cell->setResult(pprint(result));
    cell->setStatus(Cell::IDLE);
}

void Notebook::cellFailedToParse(Cell *cell, ParseResult result)
{
    qInfo() << "cellFailedToParse" << cell->uuid() << pprint(result);
    cell->setResult(pprint(result));
    cell->setStatus(Cell::IDLE);
}

void Notebook::cellWaiting(Cell *cell)
{
    qInfo() << "cellWaiting" << cell->uuid();
    cell->setStatus(Cell::WAITING);
}

void Notebook::cellRunning(Cell *cell)
{
    qInfo() << "cellRunning" << cell->uuid();
    cell->setStatus(Cell::RUNNING);
}

void Notebook::cellQuit(Cell *cell)
{
    qInfo() << "cellQuit" << cell->uuid();
    cell->setResult("");
    cell->setStatus(Cell::IDLE);
}
