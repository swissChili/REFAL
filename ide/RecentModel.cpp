#include "RecentModel.h"
#include <QSettings>

RecentModel::RecentModel(QObject *parent)
    : QAbstractListModel(parent)
{
    _recents = _settings.value("recents").toStringList();
}

RecentModel::RecentModel(const RecentModel &other, QObject *parent)
    : RecentModel(parent)
{
    _recents = other._recents;
}

int RecentModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _recents.size();
}

QVariant RecentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == PathRole)
        return _recents[index.row()];

    return QVariant();
}

QHash<int, QByteArray> RecentModel::roleNames() const
{
    return {{PathRole, "path"}};
}

void RecentModel::add(QString path)
{
    remove(path);

    beginInsertRows(QModelIndex(), 0, 0);
    _recents.prepend(path);
    endInsertRows();

    _settings.setValue("recents", _recents);
}

void RecentModel::remove(QString path)
{
    if (_recents.contains(path))
    {
        int index = _recents.indexOf(path);
        beginRemoveRows(QModelIndex(), index, index);
        _recents.removeAt(index);
        endRemoveRows();

        _settings.setValue("recents", _recents);
    }
}
