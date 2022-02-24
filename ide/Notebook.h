#pragma once

#include <QObject>

#include "Cell.h"

class CellModel;

class Notebook : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(CellModel *cellModel READ cellModel NOTIFY cellModelChanged)

public:
    explicit Notebook(QObject *parent = nullptr);
    Notebook(const Notebook &other, QObject *parent = nullptr);

    CellModel *cellModel();

signals:
    void cellModelChanged();

protected:
    friend class CellModel;

    QList<Cell> _cells;
    CellModel *_cellModel;
};

Q_DECLARE_METATYPE(Notebook)
