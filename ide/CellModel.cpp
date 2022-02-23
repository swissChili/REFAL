#include "CellModel.h"

CellModel::CellModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

CellModel::CellModel(const CellModel &model, QObject *parent)
    : CellModel(parent)
{
    _cells = model._cells;
}

int CellModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return _cells.size();
}

QVariant CellModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
    case CodeRole:
        return _cells[index.row()].code();
    case ResultRole:
        return _cells[index.row()].result();
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
            _cells[index.row()].setCode(value.toString());
            break;
        case ResultRole:
            _cells[index.row()].setResult(value.toString());
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

    return Qt::ItemIsEditable; // FIXME: Implement me!
}

bool CellModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    for (int i = 0; i < count; i++)
        _cells.insert(row, Cell());

    endInsertRows();

    return false;
}

bool CellModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; i++)
        _cells.removeAt(row);

    endRemoveRows();

    return true;
}

QHash<int, QByteArray> CellModel::roleNames() const
{
    return {
        {CodeRole, "code"},
        {ResultRole, "result"},
    };
}

void CellModel::addCell(const Cell &cell)
{
    int i = _cells.size();

    insertRows(i, 1, QModelIndex());

    _cells[i] = cell;
    emit dataChanged(index(i), index(i), {CodeRole, ResultRole});
}

void CellModel::addCell(QString code, QString result)
{
    addCell(Cell(code, result));
}
