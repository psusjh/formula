#include "stdafx.h"

#include "CodeCompiler.h"

#include <algorithm>
using namespace std;
CodeCompiler::CodeCompiler()
{
	vctLiteralTable.reserve(1024);
	vctSymbolTable.reserve(1024);
}


CodeCompiler::~CodeCompiler()
{
}

int CodeCompiler::addConst(double v){
	
	return addLiteral(Literal(v));	
}

int CodeCompiler::addString(const char* str)
{
	return addLiteral(Literal(str));
}

int CodeCompiler::addVar(const char * name)
{
	if (findVar(name) < 0)
	{
		return addLiteral(Literal(name, 0, nullptr));
	}
	return -1;
}

int CodeCompiler::findVar(const char* name)
{
	VectorLiteralTable::iterator it = find_if(vctLiteralTable.begin(), vctLiteralTable.end(), [name](Literal& literal) {
		if (literal.literalType == LITERAL_TYPE_VAR && strcmp(literal.literalValue.var.name, name) == 0) {
			return true;
		}
		return false;
	});
	if (it != vctLiteralTable.end()) {
		return it - vctLiteralTable.begin();
	}
	return -1;
}

int CodeCompiler::addLiteral(Literal literal)
{
	VectorLiteralTable::iterator it = find(vctLiteralTable.begin(), vctLiteralTable.end(), literal);
	if (it != vctLiteralTable.end()) {
		return it - vctLiteralTable.begin();
	}
	vctLiteralTable.push_back(literal);
	return vctLiteralTable.size() - 1;
}
