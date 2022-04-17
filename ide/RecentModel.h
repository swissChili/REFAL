#pragma once

#include <QAbstractListModel>
#include <qqml.h>
#include <QSettings>

class RecentModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit RecentModel(QObject *parent = nullptr);
    RecentModel(const RecentModel &other, QObject *parent = nullptr);

    enum
    {
        PathRole = Qt::UserRole + 1
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void add(QString path);
    Q_INVOKABLE void remove(QString path);

private:
    QStringList _recents;
    QSettings _settings;
};

Q_DECLARE_METATYPE(RecentModel)
Q_DECLARE_METATYPE(RecentModel *)
