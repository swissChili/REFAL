#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QFile>

#include "Cell.h"
#include "NbRuntime.h"
#include "CellModel.h"

class Notebook : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(CellModel *cellModel READ cellModel NOTIFY cellModelChanged)
    Q_PROPERTY(QString savePath READ savePath WRITE setSavePath NOTIFY savePathChanged)

public:
    ~Notebook();
    explicit Notebook(QObject *parent = nullptr);
    Notebook(const Notebook &other, QObject *parent = nullptr);

    CellModel *cellModel();

    Q_INVOKABLE void runCell(QUuid uuid);
    Q_INVOKABLE void quitCell(QUuid uuid);

    Q_INVOKABLE QJsonDocument toJson() const;
    Q_INVOKABLE void save();

    Q_INVOKABLE bool savePathSet();

    QString savePath();
    void setSavePath(QString savePath);

signals:
    void cellModelChanged();
    void saveError(QString message);
    void savePathChanged(QString savePath);

protected slots:
    void cellFinishedRunning(Cell *cell, RuntimeResult result);
    void cellFailedToParse(Cell *cell, ParseResult result, Parser parser);
    void cellWaiting(Cell *cell);
    void cellRunning(Cell *cell);
    void cellQuit(Cell *cell);

protected:
    friend class CellModel;

    QList<Cell *> _cells;
    CellModel *_cellModel;
    QThread *_rtThread;
    NbRuntime *_rt;
    QString _savePath = "";
};

Q_DECLARE_METATYPE(Notebook)
