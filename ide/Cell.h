#pragma once

#include <QObject>
#include <qqml.h>
#include <QUuid>
#include <QHash>

class Cell : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString code READ code WRITE setCode NOTIFY codeChanged)
    Q_PROPERTY(QString result READ result WRITE setResult NOTIFY resultChanged)
    Q_PROPERTY(QUuid uuid READ uuid NOTIFY uuidChanged)
    Q_PROPERTY(int status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(int resultType READ resultType WRITE setResultType NOTIFY resultTypeChanged)

public:
    ~Cell();
    explicit Cell(QObject *parent = nullptr);
    Cell(const Cell &copy, QObject *parent = nullptr);
    Cell(QString code, QString result, QObject *parent = nullptr);

    Cell &operator =(const Cell &copy);

    QString code() const;
    QString result() const;
    QUuid uuid() const;
    int status() const;
    int resultType() const;

    void setCode(QString code);
    void setResult(QString result);
    void setStatus(int status);
    void setResultType(int resultType);

    Q_INVOKABLE static Cell *cellFromUuid(QUuid uuid);

    enum Status
    {
        RUNNING,
        IDLE,
        WAITING,
    };

    Q_ENUM(Status);

    enum ResultType
    {
        EXPRESSION,
        DIAGNOSTIC
    };

    Q_ENUM(ResultType)

signals:
    void codeChanged(QString code);
    void resultChanged(QString result);
    void uuidChanged(QUuid uuid);
    void statusChanged(int status);
    void resultTypeChanged(int resultType);

private:
    int _status = IDLE, _resultType = EXPRESSION;
    QString _code, _result;
    QUuid _uuid = QUuid::createUuid();

    static QHash<QUuid, Cell *> _cellUuids;
};

Q_DECLARE_METATYPE(Cell)
