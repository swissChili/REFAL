#include "CellModel.h"

CellModel::CellModel(Notebook *parent)
    : QAbstractListModel(parent)
    , _notebook(parent)
{
}

CellModel::CellModel(const CellModel &other)
    : CellModel(other._notebook)
{
}

int CellModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return _notebook->_cells.size();
}

QVariant CellModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case CodeRole:
        return _notebook->_cells[index.row()]->code();
    case ResultRole:
        return _notebook->_cells[index.row()]->result();
    case UuidRole:
        return _notebook->_cells[index.row()]->uuid();
    case StatusRole:
        return _notebook->_cells[index.row()]->status();
    default:
        return QVariant();
    }
}

bool CellModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value)
    {
        switch (role)
        {
        case CodeRole:
            _notebook->_cells[index.row()]->setCode(value.toString());
            break;
        case ResultRole:
            _notebook->_cells[index.row()]->setResult(value.toString());
            break;
        case StatusRole:
            _notebook->_cells[index.row()]->setStatus(value.toInt());
            break;
        default:
            return false;
        }

        emit dataChanged(index, index, QVector<int>() << role);

        return true;
    }

    return false;
}

Qt::ItemFlags CellModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

bool CellModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; i++)
    {
        Cell *cell = new Cell(this);

        connect(cell, &Cell::codeChanged, this, [this, cell](QString)
        {
            announceCellChange(cell, CodeRole);
        });

        connect(cell, &Cell::resultChanged, this, [this, cell](QString)
        {
            announceCellChange(cell, ResultRole);
        });

        connect(cell, &Cell::statusChanged, this, [this, cell](int)
        {
            announceCellChange(cell, StatusRole);
        });

        _notebook->_cells.insert(row, cell);
    }

    endInsertRows();

    return false;
}

bool CellModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; i++)
    {
        delete _notebook->_cells[row];
        _notebook->_cells.removeAt(row);
    }

    endRemoveRows();

    return true;
}

QHash<int, QByteArray> CellModel::roleNames() const
{
    return {
        {CodeRole, "code"},
        {ResultRole, "result"},
        {UuidRole, "uuid"},
        {StatusRole, "status"},
    };
}

void CellModel::addCell(const Cell &cell)
{
    int i = _notebook->_cells.size();

    insertRows(i, 1, QModelIndex());

    *_notebook->_cells[i] = cell;
    emit dataChanged(index(i), index(i), {CodeRole, ResultRole});
}

void CellModel::addCell(QString code, QString result)
{
    addCell(Cell(code, result));
}

void CellModel::announceCellChange(Cell *cell, int role)
{
    // TODO: Optimize

    for (int i = 0; i < rowCount(); i++)
    {
        if (_notebook->_cells[i] == cell)
        {
            emit dataChanged(index(i), index(i), QVector<int>() << role);
            break;
        }
    }
}
