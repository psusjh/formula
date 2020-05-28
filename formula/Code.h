#pragma once

#include "InstructionSymbol.h"

#include <vector>
#include <stack>
#include <unordered_map>
using namespace std;
class Code
{
	typedef vector<VarPtr> VectorVarTable;
public:
	Code();
	~Code();
public:
	/*
	 *	添加常量数据
	 *	@v 数据
	 *	@ret 返回索引
	 */
	int addConst(double v);
	
	/*
	 *	添加字符常量
	 *	@str 字符串
	 *	@ret 返回索引
	 */
	int addString(const char* str);

	/*
	 *	添加序列变量
	 *	@name 变量名称，nullptr 的时候，添加临时变量
	 *	@ret 返回索引
	 */
	int addSeries(const char* name = nullptr);
	/*
	 *	修改临时变量的名字
	 *	@pos 要修改临时变量的索引
	 *	@name 名字
	 *	@ret 0 成功， < 0 失败
	 */
	int setVarName(int pos, const char* name);
	/*
	 *	查找变量是否存在
	 *	@name 变量名称
	 *	@ret >= 0 成功， < 0 失败
	 */
	int findVar(const char* name);

	/*
	 *	添加指令块,并且进入指令块，指令完成的时候，必须退出当前指令块，
	 *	指令块一般由复合语句产生，if for begin等语句产生
	 *	@parent 父指令块，nullptr 则使用当前指令块
	 *	@ret 返回新指令块
	 */
	BlockSymbol* addBlockAndEnter(BlockSymbol* parent = nullptr);

	/*
	 *	指令块结束，离开当前指令块
	 *	@ret 返回上一个指令块
	 */
	BlockSymbol* leaveCurrentBlock();

	/*
	 *	删除指令块
	 *	@block 待删除的指令块
	 */
	void eraseBlock(BlockSymbol* block);


	/*
	 *	添加指令
	 *	@block 分组
	 *	@instruction 指令指针
	 *	@ret 返回@block指令块下面指令索引
	 */
	size_t addInstruction(BlockSymbol* block, InstructionSymbolPtr& instruction);
	/*
	 *	添加指令,添加到root分组
	 *	@id 指令id
	 *	@paramNum 指令参数个数，参数保存在栈中
	 *		默认-1，不指定具体参数个数，由注册的指令id的参数个数
	 *	@varName 指令对应的变量名字，nullptr 为临时变量，
	 *		不为空为赋值或者转出变量
	 *	@ret 返回root指令块下面指令索引
	 */
	size_t addInstruction(uint16_t id, int paramNum = -1, const char* varName = nullptr);

	/*
	 *	添加指令到指定的分组
	 *	@id 指令id
	 *	@paramNum 指令参数个数，参数保存在栈中
	 *		默认-1，不指定具体参数个数，由注册的指令id的参数个数
	 *	@varName 指令对应的变量名字，nullptr 为临时变量，
	 *		不为空为赋值或者转出变量
	 *	@ret 返回@block指令块下面指令索引
	 */
	size_t addInstruction(BlockSymbol* block, uint16_t id, int paramNum = -1, const char* varName = nullptr);
	/*
	 *	合并指令块到当前指令块
	 *	@block 待合并的指令块
	 */
	int combineBlockToCurrentBlock(BlockSymbol* block);
	/*
	 *	每一个操作符对应的变量索引需要压栈，如+（加法）左边和右边变量需要入栈，同理函数的参数也要入栈
	 *	@pos 变量索引
	 */
	void pushStack(int pos);
	/*
	 *	变量索引出栈
	 */
	int popStack();
	/*
	 *	返回操作符及函数参数栈的总数
	 */
	int getStackSize() { return operatorStack.size(); }

	/*
	 *	返回指令符号表的大小
	 *	@block 指令所在的分组
	 */
	size_t getSymbolTableSize(BlockSymbol* block = nullptr) { 
		if (block) {
			return block->symbols.size();
		}
		return currentBlock->symbols.size();
	}
	/*
	 *	返回所有指令符号表，
	 */
	size_t getTotalSymbolTableSize(BlockSymbol* block);

	const InstructionSymbolPtr getLastInstruction(BlockSymbol* block = nullptr) { 
		if (block) {
			return *block->symbols.rbegin();
		}
		return *currentBlock->symbols.rbegin();
	}

	/*
	 *	返回指令字符串，用于Debug调试显示
	 */
	string getCodeString();
public:
	static void addOpString(uint32_t sfi, const string& op);
	static string getOpString(uint32_t sfi);
protected:

	int getConstPosOfTable(double v);
	int getStringPosOfTable(const char* str);
	size_t addVar(const VarPtr& var);
private:
	void deleteBlock(BlockSymbol* block);
protected:
	struct ParamInput {
		size_t pos;
		double defaultValue;
		double minValue;
		double maxValue;
		double step;
	};
	
	VectorVarTable varTables;				//变量列表
	BlockSymbol* rootBlock;					//根指令块
	BlockSymbol* currentBlock;				//当前活动指令块
	stack<int> operatorStack;				//操作
	static unordered_map<uint32_t, string> sfi2String;
};

