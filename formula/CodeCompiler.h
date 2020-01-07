#pragma once

#include "InstructionSymbol.h"

#include <vector>
using namespace std;
class CodeCompiler
{
	typedef vector<Literal> VectorLiteralTable;
	typedef vector<InstructionSymbol> VectorSymbolTable;
public:
	CodeCompiler();
	~CodeCompiler();
public:
	int addConst(double v);
	int addString(const char* str);

	int addVar(const char* name);
	int findVar(const char* name);

protected:
	int addLiteral(Literal literal);
protected:
	VectorLiteralTable vctLiteralTable;
	VectorSymbolTable vctSymbolTable;
};

