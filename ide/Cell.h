#pragma once

#include <QObject>
#include <qqml.h>

class Cell : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString code READ code WRITE setCode NOTIFY codeChanged)
    Q_PROPERTY(QString result READ result WRITE setResult NOTIFY resultChanged)

public:
    explicit Cell(QObject *parent = nullptr);
    Cell(const Cell &copy, QObject *parent = nullptr);
    Cell(QString code, QString result, QObject *parent = nullptr);

    Cell &operator =(const Cell &copy);

    QString code() const;
    QString result() const;

    void setCode(QString code);
    void setResult(QString result);

signals:
    void codeChanged(QString code);
    void resultChanged(QString result);

private:
    QString _code, _result;
};

Q_DECLARE_METATYPE(Cell)
