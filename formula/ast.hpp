#pragma once
#include "FormulaDef.h"

#include <cstdint>
#include <string>
#include <list>
#include <vector>
using namespace std;
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant.hpp>
#include <boost/optional.hpp>

namespace ast {
	struct Nil {};	
	
	typedef boost::variant<	Nil
		, double
		, string
		, uint32_t
	>
		BaseOperand;

	//color����
	struct ColorModifer {
		BaseOperand operand;
	};

	//ȥ��color����֮����������ζ���typeΪ����
	struct TypeModifer {
		string type;
		string value;
	};

	//���������ζ���
	typedef boost::variant<Nil
		, uint32_t
		, string
		, TypeModifer
		, boost::recursive_wrapper<ColorModifer>
	>
		ModiferStmtOperand;

	struct ModiferStmtOperandList :list<ModiferStmtOperand> {};
	//�������
	struct InputStmt {
		string name;
		vector<double> params;
	};
	struct InputStmtList :list<InputStmt> {};

	//�������
	struct VariableStmt {
		string name;
		BaseOperand operands;
	};
	struct VariableStmtList : list<VariableStmt> {};

	struct Unary;
	struct Operation; 	
	struct Expression;
	struct FunctionCall;
	//quote
	struct QuoteString : string {

	};

	struct Iditenfier : string {

	};

	//���ʽ
	typedef boost::variant<Nil
		, double
		, string
		, QuoteString
		, Iditenfier
		, uint32_t
		, SFI
		, boost::recursive_wrapper<Unary>
		, boost::recursive_wrapper<FunctionCall>
		, boost::recursive_wrapper<Expression>
		
	>
		CalcOperand;
	//һԪ���ʽ
	struct Unary
	{
		SFI sfi;
		CalcOperand operand;
	};
	//������
	struct Operation
	{
		SFI sfi;
		CalcOperand operand;
	};
	//���ʽ
	struct Expression
	{
		CalcOperand first;
		list<Operation> operationLst;
	};
	struct ExpressionList :list<Expression> {};

	

	//��������
	struct FunctionCall
	{
		SFI id;
 		list<Expression> args;
	};	
	//��ֵ�������
	enum StatementType{
		assignment,
		out
	};
	struct Assignment
	{
		StatementType type;
		string lhs;
		Expression rhs;
	};

	struct Out:Assignment{
		boost::optional<ast::ModiferStmtOperandList> modifer;
	};

	struct IfStatement;
	struct WhileStatement;
	struct ForStatement;
	struct StatementList;
	struct CompoundStatement;
	
	typedef boost::variant<Nil
		, boost::recursive_wrapper<InputStmtList>
		, boost::recursive_wrapper<VariableStmtList>
		, boost::recursive_wrapper<Out>
		, boost::recursive_wrapper<IfStatement>
		, boost::recursive_wrapper<WhileStatement>
		, boost::recursive_wrapper<ForStatement>
		, boost::recursive_wrapper<StatementList>
		, boost::recursive_wrapper<CompoundStatement>
	>
		Statement;

	struct StatementList : list<Statement> {};

	struct CompoundStatement : list<Statement> {};


	struct ThenStatement {
		Statement thenStmt;
		boost::optional<Statement> elseStmt;
	};
	struct IfStatement
	{
		Expression condition;
		Statement thenStmt;
		boost::optional<Statement> elseStmt;
	};

	struct WhileStatement
	{
		Expression condition;
		Statement body;
	};	
	enum ForType {
		TYPE_TO,
		TYPE_DOWN
	};
	struct ForStatement {
		string id;
		Expression start;
		Expression end;
		Statement body;
		ForType type;
	};
	

	typedef boost::variant<Nil
		, boost::recursive_wrapper<IfStatement>
		, boost::recursive_wrapper<WhileStatement>
		, boost::recursive_wrapper<ForStatement>
		, boost::recursive_wrapper<StatementList>
		, boost::recursive_wrapper<CompoundStatement>
	>
		ComplexStatement;

}

BOOST_FUSION_ADAPT_STRUCT(
	ast::InputStmt,
	(auto, name)
	(auto, params)
)

BOOST_FUSION_ADAPT_STRUCT(
	ast::VariableStmt,
	(auto, name)
	(auto, operands)
)

BOOST_FUSION_ADAPT_STRUCT(
	ast::ColorModifer,
	(ast::BaseOperand, operand)
)

BOOST_FUSION_ADAPT_STRUCT(
	ast::TypeModifer,
	(string, type)
	(string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
	ast::Unary,
	(SFI, sfi)
	(ast::CalcOperand, operand)
)

BOOST_FUSION_ADAPT_STRUCT(
	ast::Operation,
	(SFI, sfi)
	(ast::CalcOperand, operand)
)

BOOST_FUSION_ADAPT_STRUCT(
	ast::Expression,
	(auto, first)
	(auto, operationLst)
)

BOOST_FUSION_ADAPT_STRUCT(
	ast::FunctionCall,
	(auto, id)
 	(auto, args)
)
// 
// BOOST_FUSION_ADAPT_STRUCT(
// 	ast::Assignment,	
// 	(auto, type)
// 	(auto, lhs)
// 	(auto, rhs)
// 
// )

BOOST_FUSION_ADAPT_STRUCT(
	ast::Out,	
	(auto, type)
	(auto, lhs)
	(auto, rhs)
	(auto, modifer)
)

BOOST_FUSION_ADAPT_STRUCT(
	ast::ThenStatement,
	(auto, thenStmt)
	(auto, elseStmt)
)

BOOST_FUSION_ADAPT_STRUCT(
	ast::IfStatement,
	(auto, condition)
	(auto, thenStmt)
	(auto, elseStmt)
)

BOOST_FUSION_ADAPT_STRUCT(
	ast::WhileStatement,
	(auto, condition)
	(auto, body)
)

BOOST_FUSION_ADAPT_STRUCT(
	ast::ForStatement,
	(auto, id)
	(auto, start)
	(auto, end)
	(auto, body)
	(auto, type)
)






