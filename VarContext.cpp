#include "VarContext.h"

void VarContext::add(char t, const QString &name, const Token &value) {
    _vars.insert(name, Var{t, value});
}

char VarContext::exists(const QString &name) {
    return _vars.contains(name) ? _vars[name].t : 0;
}

Token VarContext::singleVar(const QString &name) {
    return _vars[name].value;
}

VarContext::VarContext(const VarContext &other) noexcept {
    _vars = other._vars;
}

void VarContext::add(char t, const QString &name, const QList<Token> &value) {
    _vars.insert(name,Var{t,{},value});
}

QList<Token> VarContext::expressionVar(const QString &name) {
    return _vars[name].expressionValue;
}

QDebug &operator <<(QDebug &debug, const VarContext &ctx) {
    for (const auto &name: ctx._vars.keys()) {
        char t = ctx._vars[name].t;
        auto d = debug.nospace().noquote() << t << '.' << name << ":";

        if (t != 'e')
            d.nospace() << "( " << ctx._vars[name].value << " )";
        else
            d << "( " << ctx._vars[name].expressionValue << " )";
        d << "; ";
    }

    if (ctx._vars.empty()) {
        debug << "{}";
    }

    return debug;
}
