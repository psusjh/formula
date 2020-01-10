#pragma once

#include "InstructionSymbol.h"

#include <vector>
using namespace std;
class Code
{
	typedef vector<Literal> VectorLiteralTable;
	typedef vector<InstructionSymbol> VectorSymbolTable;
public:
	Code();
	~Code();
public:
	int addConst(double v);
	
	int addString(const char* str);

	int addVar(const char* name);
	int findVar(const char* name);

protected:
	int getConstPosOfTable(double v);
	int getStringPosOfTable(const char* str);
	int addLiteral(Literal literal);
protected:
	VectorLiteralTable vctLiteralTable;
	VectorSymbolTable vctSymbolTable;
};

