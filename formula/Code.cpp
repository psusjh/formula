#include "stdafx.h"

#include "Code.h"

#include <algorithm>
using namespace std;
Code::Code()
{
	vctLiteralTable.reserve(1024);
	vctSymbolTable.reserve(1024);
}


Code::~Code()
{
}

int Code::addConst(double v){
	int pos = getConstPosOfTable(v);
	if (pos >= 0) {
		return pos;
	}
	return addLiteral(Literal(v));	
}

int Code::getConstPosOfTable(double v)
{
	int pos = -1;
	VectorLiteralTable::iterator it = find_if(vctLiteralTable.begin(), vctLiteralTable.end(), [v](Literal& literal) {
		if (literal.literalType == LITERAL_TYPE_CONST && literal.literalValue.value == v) {
			return true;
		}
		return false;
	});
	if (it != vctLiteralTable.end()) {
		pos =  it - vctLiteralTable.begin();
	}
	return pos;
}

int Code::getStringPosOfTable(const char* str)
{
	int pos = -1;
	VectorLiteralTable::iterator it = find_if(vctLiteralTable.begin(), vctLiteralTable.end(), [str](Literal& literal) {
		if (literal.literalType == LITERAL_TYPE_STR && strcmp(literal.literalValue.str, str) == 0) {
			return true;
		}
		return false;
	});
	if (it != vctLiteralTable.end()) {
		pos = it - vctLiteralTable.begin();
	}
	return pos;
}

int Code::addString(const char* str)
{
	int pos = getStringPosOfTable(str);
	if (pos >= 0) {
		return pos;
	}
	return addLiteral(Literal(str));
}

int Code::addVar(const char * name)
{
	if (findVar(name) < 0)
	{
		return addLiteral(Literal(name, 0, nullptr));
	}
	return -1;
}

int Code::findVar(const char* name)
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

int Code::addLiteral(Literal literal)
{
	VectorLiteralTable::iterator it = find(vctLiteralTable.begin(), vctLiteralTable.end(), literal);
	if (it != vctLiteralTable.end()) {
		return it - vctLiteralTable.begin();
	}
	vctLiteralTable.push_back(literal);
	return vctLiteralTable.size() - 1;
}
