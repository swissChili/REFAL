#include "Notebook.h"
#include "CellModel.h"
#include "../PPrint.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include <QDebug>

// TODO: avoid potential race condition if Cell is deleted, pass by value with same UUID instead

Notebook::~Notebook()
{
    _rtThread->quit();
    _rtThread->wait();

    delete _rt;
}

Notebook::Notebook(QObject *parent)
    : QObject(parent)
    , _cellModel(new CellModel(this))
    , _rtThread(new QThread(this))
    , _rt(new NbRuntime)
{
    connect(_rt, &NbRuntime::cellFailedToParse, this, &Notebook::cellFailedToParse);
    connect(_rt, &NbRuntime::cellFinishedRunning, this, &Notebook::cellFinishedRunning);
    connect(_rt, &NbRuntime::cellQuit, this, &Notebook::cellQuit);
    connect(_rt, &NbRuntime::cellRunning, this, &Notebook::cellRunning);
    connect(_rt, &NbRuntime::cellWaiting, this, &Notebook::cellWaiting);

    _rt->moveToThread(_rtThread);
    _rtThread->start();
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
    _rt->queueCell(Cell::cellFromUuid(uuid));
}

void Notebook::quitCell(QUuid uuid)
{
    qDebug() << "Quitting cell" << uuid;
    _rt->unqueueCell(Cell::cellFromUuid(uuid));
    _runningAll = false;
}

void Notebook::runAll()
{
    if (_cells.size() > 0)
    {
        _rt->queueCell(_cells.first());
        _runningAll = true;
    }
}

void Notebook::reset()
{
    _rt->reset();
}

void Notebook::fromJson(QJsonDocument doc)
{
    QJsonObject nb = doc.object();
    QJsonArray cellArray = nb["cells"].toArray();

    for (const QJsonValueRef &cell : cellArray)
    {
        cellModel()->insertCellBefore(cellModel()->rowCount());
        _cells.last()->fromJson(cell.toObject());
    }
}

void Notebook::open(QString path)
{
    QFile file(path);

    if (file.exists())
    {
        file.open(QFile::ReadOnly);

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);

        if (error.error == QJsonParseError::NoError)
        {
            fromJson(doc);
        }
        else
        {
            qWarning() << error.errorString();
        }

        file.close();
        setSavePath(path);
    }
    else
    {
        qWarning() << "File does not exist" << path;
    }
}

QJsonDocument Notebook::toJson() const
{
    QJsonObject nb;
    QJsonArray cellArray;

    for (const Cell *cell : _cells)
    {
        cellArray.append(cell->toJson());
    }

    nb["cells"] = cellArray;

    return QJsonDocument(nb);
}

void Notebook::save()
{
    if (_savePath == "")
    {
        setSavePath(QFileDialog::getSaveFileName(nullptr, "Open REFAL Notebook", "", "REFAL Notebook (*.refnb)"));
    }

    QJsonDocument doc = toJson();
    QFile save(_savePath);
    save.open(QFile::WriteOnly);

    if (!save.isOpen())
    {
        emit saveError(save.errorString());
        return;
    }

    save.write(doc.toJson(QJsonDocument::Indented));
    save.close();

    emit saved();
}

bool Notebook::savePathSet()
{
    return _savePath != "";
}

QString Notebook::savePath()
{
    return _savePath;
}

void Notebook::setSavePath(QString savePath)
{
    _savePath = savePath;
    emit savePathChanged(savePath);
}

void Notebook::cellFinishedRunning(Cell *cell, RuntimeResult result)
{
    qInfo() << "cellFinishedRunning" << cell->uuid() << pprint(result);
    cell->setResult(pprint(result));
    cell->setStatus(Cell::IDLE);
    cell->setResultType(Cell::EXPRESSION);

    if (_runningAll)
    {
        int index = _cells.indexOf(cell);

        // not last
        if (index < _cells.size() - 1)
        {
            _rt->queueCell(_cells[index + 1]);
        }
        else
        {
            _runningAll = false;
        }
    }
}

void Notebook::cellFailedToParse(Cell *cell, ParseResult result, Parser parser)
{
    qInfo() << "cellFailedToParse" << cell->uuid() << pprint(result, parser);
    cell->setResult(pprint(result, parser, PPrint::HTML));
    cell->setStatus(Cell::IDLE);
    cell->setResultType(Cell::DIAGNOSTIC);

    _runningAll = false;
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

    _runningAll = false;
}
