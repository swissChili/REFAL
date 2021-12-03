#include "VarContext.h"

void VarContext::add(char t, const QString &&name, const Token &value) {
    _vars.insert(name, Var{t, value});
}

char VarContext::exists(const QString &name) {
    return _vars.contains(name);
}

Token VarContext::operator[](const QString &name) {
    return _vars[name].value;
}

VarContext::VarContext(const VarContext &other) noexcept {
    _vars = other._vars;
}
