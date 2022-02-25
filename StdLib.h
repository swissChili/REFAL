#pragma once

#include "Function.h"
#include "Evaluator.h"

class StdLib {
public:
	StdLib();

	void load(Evaluator &eval);

private:
	Function _print{"Print"},
        _prout{"Prout"},
        _sleep{"Sleep"};
};
