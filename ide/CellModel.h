#pragma once

#include <QAbstractListModel>
#include <qqml.h>

#include "Cell.h"

class Notebook;

class CellModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit CellModel(Notebook *parent = nullptr);
    CellModel(const CellModel &other);

    enum CellRoles
    {
        CodeRole = Qt::UserRole + 1,
        ResultRole,
        UuidRole,
        StatusRole,
        ResultTypeRole,
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value,
                             int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    Q_INVOKABLE bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    Q_INVOKABLE bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addCell(const Cell &cell);
    Q_INVOKABLE void addCell(QString code, QString result);

    Q_INVOKABLE void insertCellBefore(int index);
    Q_INVOKABLE void deleteCellAt(int index);

private:
    Notebook *_notebook;
    void announceCellChange(Cell *cell, int role);
};

Q_DECLARE_METATYPE(CellModel)
Q_DECLARE_METATYPE(CellModel *)
