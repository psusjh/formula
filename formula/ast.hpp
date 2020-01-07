#pragma once
#include "FormulaDef.h"

#include <cstdint>
#include <string>
#include <list>
#include <vector>
using namespace std;

#include "boost/variant.hpp"
#include "boost/optional.hpp"

namespace ast {
	struct Nil {};

	struct Literal {
		Literal(const string& name=""):name(name){}
		uint32_t	id;
		string		name;
	};

	
	struct FunctionCall;
	struct Expression;


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


	//�������
	struct InputStmt {
		string name;
		vector<double> params;
	};

	//�������
	struct VariableStmt {
		string name;
		vector<BaseOperand> operands;
	};

	struct Unary;
	struct Operation;
	struct Expression;
	struct FunctionCall;
	//���ʽ
	typedef boost::variant<Nil
		, char
		, double
		, string
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
		std::list<CalcOperand> operationLst;
	};

	struct FunctionCall
	{
		string name;
// 		boost::optional<list<Expression>> args;
	};

	

// 	struct Assignment
// 	{
// 		Literal lhs;
// 		Expression rhs;
// 	};
// 
// 	struct VariableDeclaration
// 	{
// 		Literal lhs;
// 		boost::optional<Expression> rhs;
// 	};
// 
// 	struct IfStatement;
// 	struct WhileStatement;
// 	struct StatementList;
// 	struct CompoundStatement;
// 	
// 
// 	typedef boost::variant<
// 		VariableDeclaration
// 		, Assignment
// 		, boost::recursive_wrapper<IfStatement>
// 		, boost::recursive_wrapper<WhileStatement>
// 		, boost::recursive_wrapper<StatementList>
// 	>
// 		Statement;
// 
// 	struct StatementList : std::list<Statement> {};
// 
// 	struct IfStatement
// 	{
// 		Expression condition;
// 		Statement then;
// 		boost::optional<Statement> else_;
// 	};
// 
// 	struct WhileStatement
// 	{
// 		Expression condition;
// 		Statement body;
// 	};	
// 
// 	struct CompoundStatement {
// 		StatementList body;
// 	};

	
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
	(auto, name)
// 	(auto, args)
)






