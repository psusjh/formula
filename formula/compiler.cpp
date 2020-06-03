#include "stdafx.h"
#include "compiler.h"
#include "Code.h"
#include <fstream>
using namespace std;
#include "InternalFunction.h"

extern string mainPath;

void align_print(ostream& out, int depth) {
	while (depth-- > 0) {
		out << "\t";
	}
}

compiler::compiler(const shared_ptr<Code>& code)
{
	this->codeptr = code;
	depth = 0;
	curOutResult = -1;
	curCorlor = 0;
	log = make_shared<ofstream>(mainPath+"tree.txt");
	
}

compiler::compiler(shared_ptr<ofstream>& log, shared_ptr<Code>&code, int depth)
{
	this->codeptr = code;
	this->depth = depth;
	this->log = log;
	curOutResult = -1;
	curCorlor = 0;
}

std::ostream& compiler::getOut()
{
	return *log;
}

compiler::~compiler()
{

}

void compiler::operator()(ast::ColorModifer const& modifer)
{
	align_print(getOut(), depth);
	getOut() << "type:ColorModifer"
		// 			<<" "
		// 			<<color.value
		<< "\n";
	boost::apply_visitor(*this, modifer.operand);
}

void compiler::operator()(ast::TypeModifer const& typeModifer)
{
	align_print(getOut(), depth);
	getOut() << "type:typeModifer:" << typeModifer.type << " value:" << typeModifer.value << "\n";
	if (curOutResult >= 0) {
		codeptr->setOutResultModifer(typeModifer.type, typeModifer.value);
	}
}

void compiler::operator()(ast::InputStmt const& inputStmt)
{
	align_print(getOut(), depth);
	getOut() << "type:InputStmt\t"
		<< "inputId:" << inputStmt.name
		<< "params:" << inputStmt.params.size()
		<< endl;
	double defaultValue = inputStmt.params.size() ? inputStmt.params[0] : 0;
	int pos = codeptr->addConst(defaultValue);
	codeptr->setVarName(pos, inputStmt.name.c_str());
	double minValue = inputStmt.params.size() > 1 ? inputStmt.params[1] : defaultValue;
	double maxValue = inputStmt.params.size() > 2 ? inputStmt.params[2] : minValue;
	double step = inputStmt.params.size() > 3 ? inputStmt.params[3] : 0;
	codeptr->addInputParam(pos, defaultValue, minValue, maxValue, step);
}


void compiler::operator()(ast::InputStmtList const& inputStmts)
{
	align_print(getOut(), depth);
	getOut() << "type:InputStmtList\t";
	for (auto e : inputStmts) {
		this->operator()(e);
	}
}

void compiler::operator()(ast::VariableStmt const& variantStmt)
{
	getOut() << "type:VariableStmt"
		<< variantStmt.name
		<< endl;
	boost::apply_visitor(*this, variantStmt.operands);	
 	int v = codeptr->popStack();
	codeptr->setVarName(v, variantStmt.name.c_str());
}

void compiler::operator()(ast::VariableStmtList const& variantStmts)
{
	align_print(getOut(), depth);
	getOut() << "type:VariableStmtList\tsize:"<<variantStmts.size()<<endl;
	for (auto e : variantStmts) {
		this->operator()(e);
	}
}

void compiler::operator()(ast::Unary const& unary)
{
	align_print(getOut(), depth);
	getOut() << "type:Unary\t"
		<< "sfi:" << InternalFunctionSingleton::instance().getFunctionName(unary.sfi) << "\n"
		;
	boost::apply_visitor(*this, unary.operand);
	codeptr->addInstruction(unary.sfi);	
}

void compiler::operator()(ast::Operation const& operation)
{
	align_print(getOut(), depth);
	getOut() << "type:Operation\t"
		<< "sfi:" << InternalFunctionSingleton::instance().getFunctionName(operation.sfi) << "\n"
		;
	boost::apply_visitor(*this, operation.operand);
	codeptr->addInstruction(operation.sfi);
	
}

void compiler::operator()(ast::FunctionCall const& functionCall)
{
	align_print(getOut(), depth);
	++depth;
	getOut() << "type:FunctionCall" << endl;

	FunctionPtr func = InternalFunctionSingleton::instance().getFunction(functionCall.id);
	if (func->paramNum > functionCall.args.size()) {
		//传入的参数小于实际参数
		assert(0);
	}
	list<ast::Expression>::const_iterator it = functionCall.args.begin();
	for (size_t i = 0; i < func->paramNum && it != functionCall.args.end(); ++i,++it)
	{
		//处理参数
		this->operator()(*it);
	}
	
	codeptr->addInstruction(func->id);
	--depth;
}

void compiler::operator()(ast::Out const& out)
{
	align_print(getOut(), depth);
	getOut() << "type:Out-------\n";
	getOut() << "outType:" << out.type << "\n";
	getOut() << "type:Out\n" << "lhs=" << out.lhs << "\n";
	
	this->operator()(out.rhs);
	codeptr->addInstruction(out.type == ast::StatementType::assignment? SFI_ASSIGN: SFI_OUT, 1, out.lhs.c_str());
	int pos = codeptr->popStack();
	curOutResult = codeptr->addOutResult(pos);
	if (out.modifer.has_value()) {
		for (auto modifer : out.modifer.get()) {
			boost::apply_visitor(*this, modifer);
		}
	}
	curOutResult = -1;
	
	getOut() << "out-------\n";
}

void compiler::operator()(ast::CompoundStatement const& compoundStatement)
{
	align_print(getOut(), depth);
	getOut() << "type:CompoundStatement\tsize:" << compoundStatement.size() << endl;

	BlockSymbol* compoundBlock = codeptr->addBlockAndEnter();
	for (auto e : compoundStatement) {
		boost::apply_visitor(*this, e);
	}
	codeptr->leaveCurrentBlock();
	codeptr->combineBlockToCurrentBlock(compoundBlock);
	codeptr->eraseBlock(compoundBlock);
}


void compiler::operator()(ast::StatementList const& statements)
{
	align_print(getOut(), depth);
	getOut() << "type:StatementList\tsize:" << statements.size() << endl;
	for (auto e : statements) {
		boost::apply_visitor(*this, e);
	}
}



void compiler::operator()(ast::IfStatement const& ifStatement)
{
	align_print(getOut(), depth);
	getOut() << "type:IfStatement" ;
	size_t startSize = codeptr->getSymbolTableSize();
	//处理条件
	this->operator()(ifStatement.condition);
	assert(startSize < codeptr->getSymbolTableSize());
	startSize = codeptr->getSymbolTableSize();

	const InstructionSymbolPtr lastInstruction = codeptr->getLastInstruction();
	assert(lastInstruction);
	FunctionPtr fun = InternalFunctionSingleton::instance().getFunction(lastInstruction->id);
	//添加if语句,if语句是block指令块
	BlockSymbol* elseBlock = nullptr, * thenBlock = codeptr->addBlockAndEnter();
	boost::apply_visitor(*this, ifStatement.thenStmt);
	codeptr->leaveCurrentBlock();
	size_t elseSymbolCount = 0, thenSymbolCount = codeptr->getTotalSymbolTableSize(thenBlock);
	//添加else语句，else语句也是block指令块
	if (ifStatement.elseStmt) {
		elseBlock = codeptr->addBlockAndEnter();
		boost::apply_visitor(*this, *ifStatement.elseStmt);
		codeptr->leaveCurrentBlock();
		elseSymbolCount = codeptr->getTotalSymbolTableSize(elseBlock);
		getOut() << "\t has else statement\t";
	}
	getOut() << "thenCount:" << thenSymbolCount << "\t" << "elseCount:" << elseSymbolCount << endl;
	//+1包括后面一条控制指令JMP
	if (elseBlock) {
		thenSymbolCount += 1;
	}
	codeptr->pushStack(codeptr->addConst(thenSymbolCount + 1));
	//控制指令，没有结果
	codeptr->addInstruction(SFI_JNEQ);
	codeptr->popStack();
	codeptr->combineBlockToCurrentBlock(thenBlock);
	if (elseBlock) {
		codeptr->pushStack(codeptr->addConst(elseSymbolCount + 1));
		codeptr->addInstruction(SFI_JUMP);
		codeptr->popStack();
		codeptr->combineBlockToCurrentBlock(elseBlock);
	}
	codeptr->eraseBlock(thenBlock);
	codeptr->eraseBlock(elseBlock);
	getOut() << endl;
}

void compiler::operator()(ast::WhileStatement const& whileStatement)
{
	align_print(getOut(), depth);
	getOut() << "type:WhileStatement" << endl;
	size_t stackSize = codeptr->getStackSize();
	this->operator()(whileStatement.condition);
	size_t curSymbolSize = codeptr->getSymbolTableSize();

	BlockSymbol* whileBlock = codeptr->addBlockAndEnter();
	boost::apply_visitor(*this, whileStatement.body);
	codeptr->pushStack(codeptr->addConst(-(double)(codeptr->getTotalSymbolTableSize(whileBlock) + 1)));
	codeptr->addInstruction(SFI_JUMP);
	codeptr->popStack();
	codeptr->leaveCurrentBlock();


	codeptr->pushStack(codeptr->addConst(codeptr->getTotalSymbolTableSize(whileBlock) + 1));
	codeptr->addInstruction(SFI_JNEQ);
	codeptr->popStack();

	//合并到当前Block
	codeptr->combineBlockToCurrentBlock(whileBlock);

	codeptr->eraseBlock(whileBlock);
	assert(stackSize == codeptr->getStackSize());
}

void compiler::operator()(ast::ForStatement const& forStatement)
{
	align_print(getOut(), depth);
	getOut() << "type:ForStatement" << endl;

	size_t stackSize = codeptr->getStackSize();
	//start 赋值语句
	this->operator()(forStatement.start);
	codeptr->addInstruction(SFI_ASSIGN , 1, forStatement.id.c_str());
	InstructionSymbolPtr lastInstruction = codeptr->getLastInstruction();	

	//end 结束条件
	this->operator()(forStatement.end);

	size_t curSymbolSize = codeptr->getSymbolTableSize();
	//添加for语句块
	BlockSymbol* forBlock = codeptr->addBlockAndEnter();
	boost::apply_visitor(*this, forStatement.body);
	//变量+=1或者-=1；
	codeptr->pushStack(lastInstruction->resultPos);
	codeptr->pushStack(codeptr->addConst(1));
	codeptr->addInstruction(forStatement.type == ast::ForType::TYPE_TO ? SFI_PLUS : SFI_MINUS, -1, forStatement.id.c_str());
	codeptr->popStack();
	//添加JMP到开始判断语句
	codeptr->pushStack(codeptr->addConst(-(double)(codeptr->getSymbolTableSize()+1)));
	codeptr->addInstruction(SFI_JUMP);
	codeptr->popStack();
	//退出for语句块
	codeptr->leaveCurrentBlock();
	//添加条件跳转语句
	codeptr->pushStack(codeptr->addConst(codeptr->getTotalSymbolTableSize(forBlock)+1));
	codeptr->addInstruction(forStatement.type == ast::ForType::TYPE_TO ? SFI_JGT : SFI_JLT);
	codeptr->popStack();
	//合并到当前Block
	codeptr->combineBlockToCurrentBlock(forBlock);

	codeptr->eraseBlock(forBlock);
	assert(stackSize == codeptr->getStackSize());
}

void compiler::operator()(ast::Expression const& expression)
{
	align_print(getOut(), depth);
	getOut() << "type:Expression-----, depth:" << depth << " express.size:" << expression.operationLst.size() << endl;
	++depth;
	boost::apply_visitor(*this, expression.first);
	for (auto e : expression.operationLst) {
		align_print(getOut(), depth);
		getOut() << "type:Expression->child*****, depth:" << depth << "\n";
		this->operator()(e);
		align_print(getOut(), depth);
		getOut() << "*****type:Expression->child\n";
	}
	align_print(getOut(), depth);
	getOut() << "-----type:Expression\n";
	--depth;
}

void compiler::operator()(const string& x)
{
	align_print(getOut(), depth);
	if (curOutResult >= 0) {
		curCorlor = 0;
		for (size_t i = 0; i < x.size(); i += 2) {
			uint8_t rgb = strtoul(x.substr(i, 2).c_str(), nullptr, 16);
			curCorlor |= (rgb << 8 * i/2);
		}
		codeptr->setOutResultModifer("color", to_string(curCorlor));
	}
	getOut() << "type:string depth:" << depth << " value:" << x << endl;
}

void compiler::operator()(const ast::Iditenfier& x)
{
	align_print(getOut(), depth);
	
	getOut() << "type:Iditenfier depth:" << depth << " value:" << x << endl;
	int pos = codeptr->findVar(x.c_str());
	assert(pos >= 0);
	codeptr->pushStack(pos);
}

void compiler::operator()(ast::QuoteString& x)
{
	align_print(getOut(), depth);
	getOut() << "type:QuoteString depth:" << depth << " value:" << x << endl;
}

void compiler::operator()(uint32_t x)
{
	align_print(getOut(), depth);
	if (curOutResult >= 0) {
		curCorlor = x;
		codeptr->setOutResultModifer("color", to_string(curCorlor));
	}
	getOut() << "type:uint32_t depth:" << depth << " value:" << x << endl;
}

void compiler::operator()(SFI x)
{
	align_print(getOut(), depth);
	getOut() << "type:SFI depth:" << depth << " value:" << x << endl;
}

void compiler::operator()(double x)
{
	align_print(getOut(), depth);
	getOut() << "type:double depth:" << depth << " value:" << x << endl;
	codeptr->pushStack(codeptr->addConst(x));
}

void compiler::operator()(ast::Nil)
{
	align_print(getOut(), depth);
	getOut() << "type:nil\n" << endl;
}

void compiler::printCode()
{
	getOut() << codeptr->getCodeString();
}

