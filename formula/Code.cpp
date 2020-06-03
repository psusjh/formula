#include "stdafx.h"

#include "Code.h"
#include "FormulaDef.h"
#include <algorithm>
#include <sstream>
#include <assert.h>
using namespace std;
#include "InternalFunction.h"

unordered_map<uint32_t, string> Code::sfi2String;


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
	varTables.reserve(1024);
	rootBlock = new BlockSymbol();
	rootBlock->id = 0;
	rootBlock->parent = nullptr;
	currentBlock = rootBlock;
}


Code::~Code()
{
	
	deleteBlock(rootBlock);
}

int Code::addConst(double v){
	int pos = getConstPosOfTable(v);
	if (pos >= 0) {
		return pos;
	}
	return addVar(shared_ptr<Var>(new SingleNumberVar(v)));	
}

int Code::getConstPosOfTable(double v)
{
	int pos = -1;
	VectorVarTable::iterator it = find_if(varTables.begin(), varTables.end(), [v](VarPtr& var) {
		if (var->getVarType() == Var::VAR_TYPE_SINGLE 
			&& var->getDataType() == Var::DATA_TYPE_NUMBER 
			&& var->getValue() == v
			&& strlen(var->getName())<=0
			) {
			return true;
		}
		return false;
	});
	if (it != varTables.end()) {
		pos =  it - varTables.begin();
	}
	return pos;
}

int Code::getStringPosOfTable(const char* str)
{
	int pos = -1;
	VectorVarTable::iterator it = find_if(varTables.begin(), varTables.end(), [str](VarPtr& var) {
		if (
			var->getVarType() == Var::VAR_TYPE_SINGLE 
			&& var->getDataType() == Var::DATA_TYPE_STRING
			&& strcmp(var->getStringValue(),str) == 0) {
			return true;
		}
		return false;
		});
	if (it != varTables.end()) {
		pos = it - varTables.begin();
	}
	return pos;
}

int Code::addString(const char* str)
{
	int pos = getStringPosOfTable(str);
	if (pos >= 0) {
		return pos;
	}
	return addVar(shared_ptr<Var>(new SingleStringVar(str)));
}

int Code::addSeries(const char * name/* = nullptr*/)
{
	return addVar(VarPtr(new SeriesNumberVar()));
}

int Code::setVarName(int pos, const char* name)
{
	if (pos < 0 || pos >= varTables.size()) {
		assert(0);
		return -1;
	}
	varTables[pos]->setName(name);
	return 0;
}

int Code::findVar(const char* name)
{
	VectorVarTable::iterator it = find_if(varTables.begin(), varTables.end(), [name](VarPtr& var) {
		if ( name && strcmp(var->getName(), name) == 0) {
			return true;
		}
		return false;
	});
	if (it != varTables.end()) {
		return it - varTables.begin();
	}
	return -1;
}

BlockSymbol* Code::addBlockAndEnter(BlockSymbol* parent /*= nullptr*/)
{
	if (!parent) {
		assert(currentBlock);
		parent = currentBlock;
	}
	BlockSymbol* block = new BlockSymbol;
	parent->groups.push_back(block);
	block->id = parent->groups.size() - 1;
	block->parent = parent;
	currentBlock = block;
	return block;
}

BlockSymbol* Code::leaveCurrentBlock()
{
	if (currentBlock && currentBlock->parent) {
		currentBlock = currentBlock->parent;
	}
	return currentBlock;
}

void Code::eraseBlock(BlockSymbol* block)
{
	if (!block) {
		return;
	}
	vector<BlockSymbol*>::iterator itf = find(currentBlock->groups.begin(), currentBlock->groups.end(), block);
	assert(itf != currentBlock->groups.end());
	if (itf != currentBlock->groups.end()) {
		deleteBlock(block);
		currentBlock->groups.erase(itf);
	}	
}

size_t Code::addInstruction(BlockSymbol* block,InstructionSymbolPtr& instruction)
{
	block->symbols.push_back(instruction);
	return block->symbols.size() - 1;
}

size_t Code::addInstruction(uint16_t id, int paramNum/* = -1*/, const char* varName/* = nullptr*/)
{
	return addInstruction(currentBlock, id, paramNum, varName);
}

size_t Code::addInstruction(BlockSymbol* block, uint16_t id, int paramNum /*= -1*/, const char* varName /*= nullptr*/)
{
	shared_ptr<Function> fun = InternalFunctionSingleton::instance().getFunction(id);
	InstructionSymbolPtr instruction = make_shared<InstructionSymbol>();
	instruction->id = id;
	instruction->numParam = paramNum == -1 ? (fun ? fun->paramNum : 0) : paramNum;

	assert(this->operatorStack.size() >= instruction->numParam);

	instruction->literalPos = new uint32_t[instruction->numParam];
	bool seriesVar = false;
	int dataType = 0;
	for (int num = 0; num < instruction->numParam; ++num) {
		instruction->literalPos[num] = popStack();
		if (varTables[instruction->literalPos[num]]->isSeriesVar()) {
			seriesVar = true;
		}
		dataType |= varTables[instruction->literalPos[num]]->isNumberData() ? Var::DATA_TYPE_NUMBER : Var::DATA_TYPE_STRING;
	}
	Var* var = nullptr;
	if (id >= SFI_DATA_S && id < SFI_REFERENCE_FUNC || seriesVar) {
		var = new SeriesNumberVar(varName);
	}
	else {
		if (dataType == Var::DATA_TYPE_NUMBER) {
			var = new SingleNumberVar(varName);
		}
		else if (dataType == Var::DATA_TYPE_STRING) {
			var = new SingleStringVar(varName);
		}
		else {
			assert(0);
		}
	}
	instruction->resultPos = addVar(VarPtr(var));
	pushStack(instruction->resultPos);
	return addInstruction(block, instruction);
}

int Code::combineBlockToCurrentBlock(BlockSymbol* block)
{
	assert(currentBlock);
	assert(block->groups.size() == 0);
	currentBlock->symbols.insert(currentBlock->symbols.end(), block->symbols.begin(), block->symbols.end());
	return block->symbols.size();
}

void Code::pushStack(int pos)
{
	operatorStack.push(pos);
}

int Code::popStack()
{
	int pop = operatorStack.top();
	operatorStack.pop();
	return pop;
}

size_t Code::getTotalSymbolTableSize(BlockSymbol* block)
{
	size_t total = block->symbols.size();
	for (auto e : block->groups) {
		total += getTotalSymbolTableSize(e);
	}
	return total;
}

std::string Code::getCodeString()
{
	stringstream ss;
	auto getLiteralString = [&](size_t pos) {
		assert(pos >= 0 && pos < varTables.size());
		if (pos < 0 || pos >= varTables.size()) {
			return string("");
		}
		VarPtr& var = varTables[pos];
		stringstream streamVar;
		streamVar << "lit<" << pos << ">|";
		if (var->isNumberData()) {
			streamVar << " Number";
		}
		else {
			streamVar << " String";
		}
		if (strlen(var->getName()) > 0) {
			streamVar << " " << var->getName();
		}
		else {
			streamVar << " temp";
		}
		if (var->isNumberData()) {
			streamVar << "(" << var->getValue() << ")";
		}
		else {
			streamVar << "(" << var->getStringValue() << ")";
		}
			
		if (!var->isSeriesVar()) {
			streamVar << " single";
		}
		else {
			streamVar << "[]";
		}
		return streamVar.str();
	};
	
	ss << "--------------------------------------\n";
	ss << "varTables:\n";
	for (int i = 0; i < varTables.size(); ++i) {
		VarPtr& var = varTables[i];
		ss << "pos:" << i << "\tvarType:";
		if (!var->isSeriesVar()) {
			ss << "single";
		}
		else {
			ss << "series";
		}
		ss << "\tdataType:";
		if (var->isNumberData()) {
			ss << "Number";
		}
		else {
			ss << "String";
		}
		if (strlen(var->getName()) > 0) {
			ss << " " << var->getName();
		}
		else {
			ss << " temp";
		}
		if (!var->isSeriesVar()) {
			if (var->isNumberData()) {
				ss << "(" << var->getValue() << ")";
			}
			else {
				ss << "(" << var->getStringValue() << ")";
			}
		}		
		ss << "\n";		
	}
	ss << "--------------------------------------\n";
	
	ss << "**************************************\n";
	ss << "symbolTable:\n";
	vector<InstructionSymbolPtr>& symbolTables = rootBlock->symbols;
	for (int i = 0; i < symbolTables.size(); ++i) {
		uint16_t catalog = InternalFunctionSingleton::instance().getStartCatalog(symbolTables[i]->id);
		if (catalog != SFI_CONTROL) {
			ss << "pos:" << i << "\t" << getLiteralString(symbolTables[i]->resultPos) << "\t";
		}
		else {
			ss << "pos:" << i << "\tcontrol\t";
		}		
		
		shared_ptr<Function> fun = InternalFunctionSingleton::instance().getFunction(symbolTables[i]->id);
		if (catalog == SFI_CONTROL) {
			switch (fun->id) {
			case SFI_LOOP:
				ss << fun->name;
				break;
			case SFI_JUMP:
				ss << fun->name << "\t pos:" << getLiteralString(symbolTables[i]->literalPos[0]);
				break;
			case SFI_JEQ:
			case SFI_JNEQ:
				assert(symbolTables[i]->numParam == 2);
				ss << fun->name << "(" << getLiteralString(symbolTables[i]->literalPos[1]) << ")\t pos:" << getLiteralString(symbolTables[i]->literalPos[0]);
				break;
			default:
				assert(symbolTables[i]->numParam == 3);
				ss << fun->name << "("
					<< getLiteralString(symbolTables[i]->literalPos[2])
					<< ","
					<< getLiteralString(symbolTables[i]->literalPos[1])
					<< ")\t"
					<< "Pos:" << getLiteralString(symbolTables[i]->literalPos[0]);
			}
		}
		else if (catalog == SFI_OPERATOR) {
			assert(fun->paramNum == symbolTables[i]->numParam);
			if (fun->paramNum == 1) {
				ss << fun->name << "\t"<<getLiteralString(symbolTables[i]->literalPos[0])<< "\t";
			}
			else {
				ss << ":=\t";
				for (int j = fun->paramNum - 1; j >=0; j--) {
					if (j < fun->paramNum - 1) {
						ss << "\t" << fun->name << "\t";
					}
					ss << getLiteralString(symbolTables[i]->literalPos[j]);
				}
			}
		}
		else if (catalog == SFI_DATA_S) {
			ss << "->" << fun->name;
		}
		else if (catalog == SFI_REFERENCE_FUNC) {
			ss << ":=\t" << fun->name << "(";
			for (int j = fun->paramNum - 1; j >= 0; j--) {
				if (j < fun->paramNum - 1) {
					ss << ",";
				}
				ss << getLiteralString(symbolTables[i]->literalPos[j]);
			}
			ss << ")";
		}
		
		ss << "\n";
	}

	ss << "**************************************\n";
	return ss.str();
}

void Code::addInputParam(int pos, double defaultValue, double minValue, double maxValue, double step)
{
	inputParamVector.emplace_back(pos, defaultValue, minValue, maxValue, step);
}

int Code::addOutResult(int pos)
{
	Out out;
	out.pos = pos;
	out.hasModifer = false;
	outResult.emplace_back(out);
	return outResult.size() - 1;

}

void Code::setOutResultModifer(const string& type, const string& value)
{
	Out& out = *outResult.rbegin();
	out.hasModifer = true;
	if (type.compare("color") == 0) {
		out.modifer.color = atol(value.c_str());
	}
	else if (type.compare("linethick") == 0) {
		out.modifer.thick = atoi(value.c_str());
	}
	else if (type.compare("layer") == 0) {
		out.modifer.layer = atoi(value.c_str());
	}
	else if (type.compare("precise") == 0) {
		out.modifer.precise = atoi(value.c_str());
	}
	else if (type.compare("align") == 0) {
		out.modifer.align = atoi(value.c_str());
	}
	else if (type.compare("valign") == 0) {
		out.modifer.valign = atoi(value.c_str());
	}
	else if (type.compare("hideval") == 0) {
		out.modifer.hideval = true;
	}
	else if (type.compare("move") == 0) {
		out.modifer.move = atoi(value.c_str());
	}
}

size_t Code::addVar(const VarPtr& var)
{
	VectorVarTable::iterator it = find_if(
		varTables.begin(), varTables.end(), [&](VarPtr& v) {
			if (strlen(v->getName()) && stricmp(v->getName(), var->getName()) == 0) {
				return true;
			}
			return false;
		}
	);
	if (it != varTables.end()) {
		return it - varTables.begin();
	}
	varTables.push_back(var);
	return varTables.size() - 1;
}

void Code::deleteBlock(BlockSymbol* block)
{
	if (block) {
		for (size_t i = 0; i < block->groups.size(); ++i) {
			deleteBlock(block->groups[i]);
		}
		delete block;
	}
}
