#pragma once

#include "Token.h"
#include "VarContext.h"

struct MatchResult
{
    bool success;
    VarContext context;
};

template <typename T>
bool listStartsWith(const QList<T> &haystack, const QList<T> &needle)
{
    if (needle.length() > haystack.length())
        return false;

    for (int i = 0; i < needle.length(); i++)
    {
        if (haystack[i] != needle[i])
            return false;
    }

    return true;
}

template <typename T>
void listDrop(QList<T> &list, int n)
{
    for (; n; n--)
    {
        list.removeFirst();
    }
}

template <typename T>
QList<T> listSlice(QList<T> &list, int from, int to)
{
    QList<T> prime;

    // I guess we'll just panic if it's too long
    // TODO: ERROR HANDLING
    for (int i = 0; i < to - from; i++)
    {
        prime.append(list[from + i]);
    }

    return prime;
}

MatchResult match(QList<Token> data, QList<Token> pattern, VarContext context);
