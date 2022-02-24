#include "Notebook.h"
#include "CellModel.h"

Notebook::Notebook(QObject *parent)
    : QObject(parent)
    , _cellModel(new CellModel(this))
{

}

Notebook::Notebook(const Notebook &other, QObject *parent)
    : QObject(parent)
    , _cells(other._cells)
    , _cellModel(new CellModel(this))
{
}

CellModel *Notebook::cellModel()
{
    return _cellModel;
}
