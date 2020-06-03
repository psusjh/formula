#pragma once

#include <iostream>
#include <memory>
using namespace std;

#include "ast.hpp"

//#include "Code.h"
class Code;

extern void align_print(ostream& out, int depth);
struct compiler :boost::static_visitor<>
{

private:
	compiler(shared_ptr<ofstream>& log, shared_ptr<Code>&code, int depth);
	ostream& getOut();
public:
	compiler(const shared_ptr<Code>& code);
	virtual ~compiler();

	void operator()(ast::Nil);

	void operator()(double x);
	void operator()(SFI x);

	void operator()(uint32_t x);

	void operator()(ast::QuoteString& x);

	void operator()(const ast::Iditenfier& x);

	void operator()(const string& x);

	void operator()(ast::Expression const& expression);

	void operator()(ast::ForStatement const& forStatement);

	void operator()(ast::WhileStatement const& whileStatement);

	void operator()(ast::IfStatement const& ifStatement);

	void operator()(ast::CompoundStatement const& compoundStatement);

	void operator()(ast::StatementList const& statements);

	void operator()(ast::Out const& out);

	void operator()(ast::FunctionCall const& functionCall);

	void operator()(ast::Operation const& operation);

	void operator()(ast::Unary const& unary);

	void operator()(ast::VariableStmtList const& variantStmts);

	void operator()(ast::VariableStmt const& variantStmt);

	void operator()(ast::InputStmtList const& inputStmts);

	void operator()(ast::InputStmt const& inputStmt);

	void operator()(ast::TypeModifer const& typeModifer);

	void operator()(ast::ColorModifer const& modifer);

	void printCode();

private:
	shared_ptr<ofstream> log;
	shared_ptr<Code> codeptr;
	int depth;
	int curOutResult;
	uint32_t curCorlor;
};
