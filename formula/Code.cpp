#include "stdafx.h"

#include "Code.h"
#include "FormulaDef.h"
#include <algorithm>
#include <sstream>
using namespace std;



void Code::addOpString(uint32_t sfi, const string& op)
{
	sfi2String.insert(make_pair(sfi, op));
}

string Code::getOpString(uint32_t sfi)
{
	unordered_map<uint32_t, string>::const_iterator itf = sfi2String.find(sfi);
	if (itf != sfi2String.end()) {
		return itf->second;
	}
	return to_string(sfi);
}

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
		if (literal.literalType == LITERAL_TYPE_VAR && name && strcmp(literal.literalValue.var.name, name) == 0) {
			return true;
		}
		return false;
	});
	if (it != vctLiteralTable.end()) {
		return it - vctLiteralTable.begin();
	}
	return -1;
}

int Code::addInstruction(InstructionSymbolPtr& instruction)
{
	vctSymbolTable.push_back(instruction);
	return vctSymbolTable.size() - 1;
}

void Code::pushQueue(int pos)
{
	literalQueue.push(pos);
}

int Code::popQueue()
{
	int pop = literalQueue.front();
	literalQueue.pop();
	return pop;
}

std::string Code::getCodeString()
{
	stringstream ss;
	
	ss << "--------------------------------------\n";
	ss << "literalTable:\n";
	for (int i = 0; i < vctLiteralTable.size(); ++i) {
		ss << "pos:" << i << "\ttype:" << vctLiteralTable[i].literalType;
		switch(vctLiteralTable[i].literalType){
		case LITERAL_TYPE_CONST:
			ss << "\tconstValue:" << vctLiteralTable[i].literalValue.value;
			break;
		case LITERAL_TYPE_STR:
			ss << "\tstrValue:" << vctLiteralTable[i].literalValue.str;
			break;
		case LITERAL_TYPE_VAR:
			ss << "\tvar";
		}
		ss << "\n";		
	}
	ss << "--------------------------------------\n";
	
	ss << "**************************************\n";
	ss << "symbolTable:\n";
	for (int i = 0; i < vctSymbolTable.size(); ++i) {
		ss << "pos:" << i << "\tlit<" << vctSymbolTable[i]->resultPos << "> :=";
		ss << "\top:" << vctSymbolTable[i]->id << "\t(";
		for (int j = 0; j < vctSymbolTable[i]->numParam; ++j) {
			ss << "lit<" << vctSymbolTable[i]->literalPos[j] << ">\t";
		}
		ss << ")\n";
	}

	ss << "**************************************\n";
	return ss.str();
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
