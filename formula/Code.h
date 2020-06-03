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

	/**
	 *	添加Input参数
	 *	@pos 参数名字在参数表的位置
	 *	@defaultValue 默认值
	 *	@minValue	最小值
	 *	@maxValue	最大值
	 *	@step	增量步长
	 */
	void addInputParam(int pos, double defaultValue, double minValue, double maxValue, double step);

	/**
	 *	添加输出结果，公式的输出语句
	 *	@pos 公式输出变量在变量表位置
	 *	@ret 返回输出结果数组的位置
	 */
	int addOutResult(int pos);

	/**
	 *	设置输出结果的修饰类型
	 *	@type 类型，color, thick, layer, precise,...,
	 *	@value 具体的值
	 */
	void setOutResultModifer(const string& type, const string& value);

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
		/**
		 *	Input参数定义
		 *	@p 参数在符号表的位置
		 *	@d 参数默认值
		 *	@mn 参数最小值 
		 *	@mx 参数最大值
		 *	@s 参数调整步长
		 */
		ParamInput(size_t p, double d, double mn, double mx, double s) {
			this->pos = p;
			this->defaultValue = d;
			this->minValue = mn;
			this->maxValue = mx;
			this->step = s;
		}
		size_t pos;
		double defaultValue;
		double minValue;
		double maxValue;
		double step;
	};
	
	struct OutModifer {	
		OutModifer() {
			thick = 1;
			layer = 0;
			precise = -1;
			align = 0;
			move = 0;
			hideval = false;
			color = 0;
		}
		char thick;			//线条宽度[0-7]
		char layer;			//图层[0-7]
		char precise;		//精度[0-7]
		char align;			//水平对齐[0-2]
		char valign;		//垂直对齐[0-2]		
		char move;			//移动位置[0-9]
		bool hideval;		//是否显示,默认false, 设置则为true,不显示
		size_t color;		//颜色
	};
	struct Out {
		size_t pos;			//符号位置
		bool hasModifer;	//是否存在输出修饰
		OutModifer modifer;	//修饰
	};
	//输出结果数组
	vector<Out> outResult;
	vector<ParamInput> inputParamVector;
	VectorVarTable varTables;				//变量列表
	BlockSymbol* rootBlock;					//根指令块
	BlockSymbol* currentBlock;				//当前活动指令块
	stack<int> operatorStack;				//操作
	static unordered_map<uint32_t, string> sfi2String;
};

