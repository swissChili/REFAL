#pragma once

#include <QObject>

#include "Cell.h"
#include "NbRuntime.h"
#include "CellModel.h"

class Notebook : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(CellModel *cellModel READ cellModel NOTIFY cellModelChanged)

public:
    ~Notebook();
    explicit Notebook(QObject *parent = nullptr);
    Notebook(const Notebook &other, QObject *parent = nullptr);

    CellModel *cellModel();

    Q_INVOKABLE void runCell(QUuid uuid);
    Q_INVOKABLE void quitCell(QUuid uuid);

signals:
    void cellModelChanged();

protected slots:
    void cellFinishedRunning(Cell *cell, RuntimeResult result);
    void cellFailedToParse(Cell *cell, ParseResult result);
    void cellWaiting(Cell *cell);
    void cellRunning(Cell *cell);
    void cellQuit(Cell *cell);

protected:
    friend class CellModel;

    QList<Cell *> _cells;
    CellModel *_cellModel;
    NbRuntime *_rt;
    QThread *_rtThread;
};

Q_DECLARE_METATYPE(Notebook)
