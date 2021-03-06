#include "Cell.h"

QHash<QUuid, Cell *> Cell::_cellUuids = QHash<QUuid, Cell*>();

Cell::~Cell()
{
    _cellUuids.remove(_uuid);
}

Cell::Cell(QObject *parent) : QObject(parent)
{
    _cellUuids[_uuid] = this;
}

Cell::Cell(const Cell &copy, QObject *parent)
    : Cell(parent)
{
    *this = copy;
}

Cell::Cell(QString code, QString result, QObject *parent)
    : Cell(parent)
{
    setCode(code);
    setResult(result);
}

Cell &Cell::operator =(const Cell &copy)
{
    setCode(copy.code());
    setResult(copy.result());

    return *this;
}

QString Cell::code() const
{
    return _code;
}

QString Cell::result() const
{
    return _result;
}

QUuid Cell::uuid() const
{
    return _uuid;
}

int Cell::status() const
{
    return _status;
}

int Cell::resultType() const
{
    return _resultType;
}

void Cell::setCode(QString code)
{
    _code = code;
    emit codeChanged(code);
}

void Cell::setResult(QString result)
{
    _result = result;
    emit resultChanged(result);
}

void Cell::setStatus(int status)
{
    _status = status;
    emit statusChanged(status);
}

void Cell::setResultType(int resultType)
{
    _resultType = resultType;
    emit resultTypeChanged(resultType);
}

Cell *Cell::cellFromUuid(QUuid uuid)
{
    if (_cellUuids.contains(uuid))
        return _cellUuids[uuid];
    else
        return nullptr;
}

void Cell::fromJson(QJsonObject json)
{
    setCode(json["code"].toString());
    setResult(json["result"].toString());
    setStatus(json["status"].toInt());
    setResultType(json["resultType"].toInt());
}

QJsonObject Cell::toJson() const
{
    QJsonObject object;

    object["code"] = code();
    object["result"] = result();
    object["status"] = status();
    object["resultType"] = resultType();

    return object;
}
