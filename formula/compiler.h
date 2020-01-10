#pragma once

#include <iostream>
#include <memory>
using namespace std;

#include "ast.hpp"

//#include "Code.h"
class Code;

extern void align_print(int depth);
struct compiler :boost::static_visitor<>
{

private:
	compiler(shared_ptr<Code>&code, int depth);
public:
	compiler(shared_ptr<Code>& code);
	virtual ~compiler();

	void operator()(ast::Nil)
	{
		align_print(depth);
		cout << "type:nil\n" << endl;
	}

	void operator()(double x)
	{
		align_print(depth);
		cout << "type:double value:" << x << endl;
		//return codeptr->addConst(x);
	}
	void operator()(SFI x)
	{
		align_print(depth);
		cout << "type:SFI value:" << x << endl;
	}

	void operator()(uint32_t x)
	{
		align_print(depth);
		cout << "type:uint32_t value:" << x << endl;
	}

	void operator()(ast::QuoteString& x)
	{
		align_print(depth);
		cout << "type:QuoteString value:" << x << endl;
	}

	void operator()(const string& x)
	{
		align_print(depth);
		cout << "type:string value:" << x << endl;
	}

	void operator()(ast::Expression const& expression)
	{
		align_print(depth);
		cout << "type:Expression" << endl;
		++depth;
		boost::apply_visitor(*this, expression.first);
		for (auto e : expression.operationLst) {
			//boost::apply_visitor(*this, e);
		}
		--depth;
	}

	void operator()(ast::ForStatement const& forStatement)
	{
		align_print(depth);
		cout << "type:ForStatement" << endl;
	}

	void operator()(ast::WhileStatement const& whileStatement)
	{
		align_print(depth);
		cout << "type:WhileStatement" << endl;
	}

	void operator()(ast::IfStatement const& ifStatement)
	{
		align_print(depth);
		cout << "type:IfStatement" << endl;
	}

	void operator()(ast::StatementList const& statements)
	{
		align_print(depth);
		cout << "type:StatementList" << endl;
		for (auto e : statements) {
		}
	}

	void operator()(ast::Out const& out)
	{
		align_print(depth);
		cout << "type:Out" << endl;
		// 	compiler compiler_(codeptr);
		// 	compiler_(out.rhs);
		// 	codeptr->addVar(out.lhs.c_str());
	}

	void operator()(ast::FunctionCall const& functionCall)
	{
		align_print(depth);
		++depth;
		cout << "type:FunctionCall" << endl;


		for (auto e : functionCall.args) {
			cout << "\tsub:";
			// 		compiler sub;
			// 		sub(e);
		}
		--depth;

		//boost::apply_visitor(*this, operation.operand);
	}

	void operator()(ast::Operation const& operation)
	{
		cout << "type:Operation\n"
			<< "sfi:" << operation.sfi << "\n"
			;
		boost::apply_visitor(*this, operation.operand);
	}

	void operator()(ast::Unary const& unary)
	{
		cout << "type:Unary\n"
			<< "sfi:" << unary.sfi << "\n"
			;
		boost::apply_visitor(*this, unary.operand);
	}

	void operator()(ast::VariableStmtList const& variantStmts)
	{
		cout << "type:VariableStmtList\n";
		for (auto e : variantStmts) {
			// 		compiler sub;
			// 		cout << "\t";
			// 		sub(e);
		}
	}

	void operator()(ast::VariableStmt const& variantStmt)
	{
		cout << "type:VariableStmt"
			<< variantStmt.name
			<< endl;
		for (auto e : variantStmt.operands) {
			boost::apply_visitor(*this, e);
		}
	}

	void operator()(ast::InputStmtList const& inputStmts)
	{
		cout << "type:InputStmtList\n";
		for (auto e : inputStmts) {
			// 		compiler sub;
			// 		cout << "\t";
			// 		sub(e);
		}
	}

	void operator()(ast::InputStmt const& inputStmt)
	{
		cout << "type:InputStmt\n"
			<< "inputId:" << inputStmt.name
			<< "params:" << inputStmt.params.size()
			<< endl;
	}

	void operator()(ast::TypeModifer const& typeModifer)
	{
		cout << "type:typeModifer:" << typeModifer.type << " value:" << typeModifer.value << "\n";
	}

	void operator()(ast::ColorModifer const& modifer)
	{
		cout << "type:ColorModifer"
			// 			<<" "
			// 			<<color.value
			<< "\n";
		boost::apply_visitor(*this, modifer.operand);
	}

private:
	shared_ptr<Code> codeptr;
	int depth;
};
