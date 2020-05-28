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
	 *	��ӳ�������
	 *	@v ����
	 *	@ret ��������
	 */
	int addConst(double v);
	
	/*
	 *	����ַ�����
	 *	@str �ַ���
	 *	@ret ��������
	 */
	int addString(const char* str);

	/*
	 *	������б���
	 *	@name �������ƣ�nullptr ��ʱ�������ʱ����
	 *	@ret ��������
	 */
	int addSeries(const char* name = nullptr);
	/*
	 *	�޸���ʱ����������
	 *	@pos Ҫ�޸���ʱ����������
	 *	@name ����
	 *	@ret 0 �ɹ��� < 0 ʧ��
	 */
	int setVarName(int pos, const char* name);
	/*
	 *	���ұ����Ƿ����
	 *	@name ��������
	 *	@ret >= 0 �ɹ��� < 0 ʧ��
	 */
	int findVar(const char* name);

	/*
	 *	���ָ���,���ҽ���ָ��飬ָ����ɵ�ʱ�򣬱����˳���ǰָ��飬
	 *	ָ���һ���ɸ�����������if for begin��������
	 *	@parent ��ָ��飬nullptr ��ʹ�õ�ǰָ���
	 *	@ret ������ָ���
	 */
	BlockSymbol* addBlockAndEnter(BlockSymbol* parent = nullptr);

	/*
	 *	ָ���������뿪��ǰָ���
	 *	@ret ������һ��ָ���
	 */
	BlockSymbol* leaveCurrentBlock();

	/*
	 *	ɾ��ָ���
	 *	@block ��ɾ����ָ���
	 */
	void eraseBlock(BlockSymbol* block);


	/*
	 *	���ָ��
	 *	@block ����
	 *	@instruction ָ��ָ��
	 *	@ret ����@blockָ�������ָ������
	 */
	size_t addInstruction(BlockSymbol* block, InstructionSymbolPtr& instruction);
	/*
	 *	���ָ��,��ӵ�root����
	 *	@id ָ��id
	 *	@paramNum ָ���������������������ջ��
	 *		Ĭ��-1����ָ�����������������ע���ָ��id�Ĳ�������
	 *	@varName ָ���Ӧ�ı������֣�nullptr Ϊ��ʱ������
	 *		��Ϊ��Ϊ��ֵ����ת������
	 *	@ret ����rootָ�������ָ������
	 */
	size_t addInstruction(uint16_t id, int paramNum = -1, const char* varName = nullptr);

	/*
	 *	���ָ�ָ���ķ���
	 *	@id ָ��id
	 *	@paramNum ָ���������������������ջ��
	 *		Ĭ��-1����ָ�����������������ע���ָ��id�Ĳ�������
	 *	@varName ָ���Ӧ�ı������֣�nullptr Ϊ��ʱ������
	 *		��Ϊ��Ϊ��ֵ����ת������
	 *	@ret ����@blockָ�������ָ������
	 */
	size_t addInstruction(BlockSymbol* block, uint16_t id, int paramNum = -1, const char* varName = nullptr);
	/*
	 *	�ϲ�ָ��鵽��ǰָ���
	 *	@block ���ϲ���ָ���
	 */
	int combineBlockToCurrentBlock(BlockSymbol* block);
	/*
	 *	ÿһ����������Ӧ�ı���������Ҫѹջ����+���ӷ�����ߺ��ұ߱�����Ҫ��ջ��ͬ�����Ĳ���ҲҪ��ջ
	 *	@pos ��������
	 */
	void pushStack(int pos);
	/*
	 *	����������ջ
	 */
	int popStack();
	/*
	 *	���ز���������������ջ������
	 */
	int getStackSize() { return operatorStack.size(); }

	/*
	 *	����ָ����ű�Ĵ�С
	 *	@block ָ�����ڵķ���
	 */
	size_t getSymbolTableSize(BlockSymbol* block = nullptr) { 
		if (block) {
			return block->symbols.size();
		}
		return currentBlock->symbols.size();
	}
	/*
	 *	��������ָ����ű�
	 */
	size_t getTotalSymbolTableSize(BlockSymbol* block);

	const InstructionSymbolPtr getLastInstruction(BlockSymbol* block = nullptr) { 
		if (block) {
			return *block->symbols.rbegin();
		}
		return *currentBlock->symbols.rbegin();
	}

	/*
	 *	����ָ���ַ���������Debug������ʾ
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
	
	VectorVarTable varTables;				//�����б�
	BlockSymbol* rootBlock;					//��ָ���
	BlockSymbol* currentBlock;				//��ǰ�ָ���
	stack<int> operatorStack;				//����
	static unordered_map<uint32_t, string> sfi2String;
};

