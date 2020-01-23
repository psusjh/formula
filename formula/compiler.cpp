#include "stdafx.h"
#include "compiler.h"
#include "Code.h"
#include <fstream>
using namespace std;

extern string mainPath;

void align_print(ostream& out, int depth) {
	while (depth-- > 0) {
		out << "\t";
	}
}

compiler::compiler(shared_ptr<Code>& code)
{
	this->codeptr = code;
	depth = 0;
	log = make_shared<ofstream>(mainPath+"tree.txt");
	
}

compiler::compiler(shared_ptr<ofstream>& log, shared_ptr<Code>&code, int depth)
{
	this->codeptr = code;
	this->depth = depth;
	this->log = log;
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
}

void compiler::operator()(ast::InputStmt const& inputStmt)
{
	align_print(getOut(), depth);
	getOut() << "type:InputStmt\t"
		<< "inputId:" << inputStmt.name
		<< "params:" << inputStmt.params.size()
		<< endl;
}

void compiler::operator()(ast::InputStmtList const& inputStmts)
{
	align_print(getOut(), depth);
	getOut() << "type:InputStmtList\t";
	for (auto e : inputStmts) {
		// 		compiler sub;
		// 		getOut() << "\t";
		// 		sub(e);
	}
}

void compiler::operator()(ast::VariableStmt const& variantStmt)
{
	getOut() << "type:VariableStmt"
		<< variantStmt.name
		<< endl;
	for (auto e : variantStmt.operands) {
		boost::apply_visitor(*this, e);
	}
}

void compiler::operator()(ast::VariableStmtList const& variantStmts)
{
	align_print(getOut(), depth);
	getOut() << "type:VariableStmtList\t";
	for (auto e : variantStmts) {
		// 		compiler sub;
		// 		getOut() << "\t";
		// 		sub(e);
	}
}

void compiler::operator()(ast::Unary const& unary)
{
	align_print(getOut(), depth);
	getOut() << "type:Unary\t"
		<< "sfi:" << unary.sfi << "\n"
		;
	boost::apply_visitor(*this, unary.operand);
}

void compiler::operator()(ast::Operation const& operation)
{
	align_print(getOut(), depth);
	getOut() << "type:Operation\t"
		<< "sfi:" << operation.sfi << "\n"
		;
	boost::apply_visitor(*this, operation.operand);

	InstructionSymbolPtr instruction = make_shared<InstructionSymbol>();
	codeptr->addInstruction(instruction);
	instruction->id = operation.sfi;
	instruction->numParam = codeptr->queueSize();
	instruction->literalPos = new uint32_t[instruction->numParam];
	for (int num = 0; num < instruction->numParam; ++num) {
		instruction->literalPos[num] = codeptr->popQueue();
	}
	int result = codeptr->addVar(nullptr);
	codeptr->pushQueue(result);
	instruction->resultPos = result;
}

void compiler::operator()(ast::FunctionCall const& functionCall)
{
	align_print(getOut(), depth);
	++depth;
	getOut() << "type:FunctionCall" << endl;


	for (auto e : functionCall.args) {
		getOut() << "\tsub:";
		// 		compiler sub;
		// 		sub(e);
	}
	--depth;

	//boost::apply_visitor(*this, operation.operand);
}

void compiler::operator()(ast::Out const& out)
{
	align_print(getOut(), depth);
	getOut() << "type:Out-------\n";
	getOut() << "outType:" << out.type << "\n";
	getOut() << "type:Out\n" << "lhs=" << out.lhs << "\n";

	compiler child(log, codeptr, depth);
	child(out.rhs);
	getOut() << "out-------\n";


	// 	compiler compiler_(codeptr);
	// 	compiler_(out.rhs);
	// 	codeptr->addVar(out.lhs.c_str());
}

void compiler::operator()(ast::StatementList const& statements)
{
	align_print(getOut(), depth);
	getOut() << "type:StatementList" << endl;
	for (auto e : statements) {
	}
}

void compiler::operator()(ast::IfStatement const& ifStatement)
{
	align_print(getOut(), depth);
	getOut() << "type:IfStatement" << endl;
}

void compiler::operator()(ast::WhileStatement const& whileStatement)
{
	align_print(getOut(), depth);
	getOut() << "type:WhileStatement" << endl;
}

void compiler::operator()(ast::ForStatement const& forStatement)
{
	align_print(getOut(), depth);
	getOut() << "type:ForStatement" << endl;
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
		compiler child(log, codeptr, depth);
		child(e);
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
	getOut() << "type:string depth:" << depth << " value:" << x << endl;
}

void compiler::operator()(ast::QuoteString& x)
{
	align_print(getOut(), depth);
	getOut() << "type:QuoteString depth:" << depth << " value:" << x << endl;
}

void compiler::operator()(uint32_t x)
{
	align_print(getOut(), depth);
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
	codeptr->pushQueue(codeptr->addConst(x));
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

