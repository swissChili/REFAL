#include "Cell.h"

Cell::Cell(QObject *parent) : QObject(parent)
{

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
