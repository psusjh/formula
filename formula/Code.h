#pragma once

#include "InstructionSymbol.h"

#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;
class Code
{
	typedef vector<Literal> VectorLiteralTable;
	typedef vector<InstructionSymbolPtr> VectorSymbolTable;
public:
	Code();
	~Code();
public:
	int addConst(double v);
	
	int addString(const char* str);

	int addVar(const char* name);
	int findVar(const char* name);
	int addInstruction(InstructionSymbolPtr& instruction);
	
	void pushQueue(int pos);
	int popQueue();
	int queueSize() { return literalQueue.size(); }

	string getCodeString();
public:
	static void addOpString(uint32_t sfi, const string& op);
	static string getOpString(uint32_t sfi);
protected:
	int getConstPosOfTable(double v);
	int getStringPosOfTable(const char* str);
	int addLiteral(Literal literal);
protected:
	VectorLiteralTable vctLiteralTable;
	VectorSymbolTable vctSymbolTable;
	queue<int> literalQueue;
	static unordered_map<uint32_t, string> sfi2String;
};

