#pragma once


#include <boost/spirit/include/phoenix.hpp>


#include "SkipperGrammar.hpp"
#include "ExpressionGrammar.hpp"
#include "ErrorGrammarHandler.hpp"
#include "ModifierSymbols.h"
#include "Code.h"
#include "ast.hpp"


template<typename Iterator>
class StatementGrammar : public qi::grammar<Iterator, ast::StatementList(), SkipperGrammar<Iterator>>
{
	typedef qi::rule<Iterator,  SkipperGrammar<Iterator>> Rule;	
public:
	StatementGrammar(ErrorGrammarHandler<Iterator>& errorHandler)
		:StatementGrammar::base_type(statementListRule, "StatementGrammar")
		, expressionGrammar(errorHandler)
	{
		using boost::phoenix::function;

		typedef function< ErrorGrammarHandler<Iterator> > FunctionErrorGrammarHandler;

		qi::_1_type _1;
		qi::_2_type _2;
		qi::_3_type _3;
		qi::_4_type _4;
		using qi::_val;

		using qi::char_;
		using qi::double_;

		using qi::eol;
		using qi::eoi;
		using qi::lit;
		using qi::alnum;
		using qi::eps;
		using qi::on_error;
		using qi::as_string;
		using qi::no_case;
		using qi::repeat;
		using qi::lexeme;
		using qi::raw;
		using qi::attr;
		using boost::phoenix::at_c;
		using boost::phoenix::push_back;
		using qi::matches;

		commaSign = char_(',');
		semicolonSign = char_(';');
		equalSign = char_('=');
		leftBracket = char_('(');
		rightBracket = char_(')');

		
		colorModifer %= lexeme[
			no_case["color"]
				>> (no_case[colorSymbols] | repeat(6)[no_case[char_("0-9a-f")]])
		];
		
		thickKey = string("linethick");
		thickNum = char_("0-7");
		thickModifer = thickKey > thickNum;

		layerKey = string("layer");
		layerNum = char_("0-7");
		layerModifer = layerKey > layerNum;
 		
		preciseKey = string("precise");
		preciseNum = char_("0-7");
 		preciseModifer = preciseKey > preciseNum;
 		
		alignKey = string("align");
		alignNum = char_("0-2");
 		alignModifer = alignKey > alignNum;

		valignKey = string("valign");
		valignNum = char_("0-2");
 		valignModifer = valignKey > valignNum;

		hideKey = string("hideval");
		hideNum = attr("1");
 		hideModifer = hideKey > hideNum;

		moveKey = string("move");
		moveNum = +char_("0-9");
 		moveModifer = moveKey > moveNum;

		modiferStmt %= colorModifer
			| thickModifer
			| layerModifer
			| preciseModifer
			| alignModifer
			| valignModifer
			| hideModifer
			| moveModifer
			| lineTypeSymbols
			;

		modiferList %= -(modiferStmt % commaSign);

		inputParamList %= +(double_ % commaSign);
 		inputSubStmt %= expressionGrammar.identifierRule > leftBracket >> inputParamList > rightBracket;
 		inputStmtRule %= no_case["input:"] >> -(inputSubStmt % commaSign) > semicolonSign;

		variableValue = double_ | expressionGrammar.stringRule;
		variableSubStmt = expressionGrammar.identifierRule > equalSign > variableValue;
		variableStmtRule = no_case["variable:"] >> -(variableSubStmt % commaSign) > semicolonSign;

		out = expressionGrammar.identifierRule[at_c<1>(_val) = _1] >(
			(lit(":=")[at_c<0>(_val) = ast::StatementType::assignment] > expressionGrammar[at_c<2>(_val) = _1] > semicolonSign)
			|(lit(":")[at_c<0>(_val) = ast::StatementType::out] > expressionGrammar[at_c<2>(_val) = _1] > -(commaSign > modiferList)[at_c<3>(_val) = _1] > semicolonSign)
			)
			;

		
		ifKey = no_case["if"];
		elseKey = no_case["else"];
		thenKey = no_case["then"];

		ifStmlRule = ifKey
					> expressionGrammar
					> thenKey
					> statementRule
					> -(elseKey > statementRule);

		whileKey = no_case["while"];
		doKey = no_case["do"];
		forKey = no_case["for"];
		toKey = no_case["to"];
		downToKey = no_case["downto"];

 		whileStmlRule = whileKey > expressionGrammar > doKey > statementRule;
 
		forStmlRule = forKey
				> expressionGrammar.identifierRule[at_c<0>(_val) = _1]
				> equalSign
				> expressionGrammar[at_c<1>(_val) = _1]
				> (downToKey[at_c<4>(_val) = ast::ForType::TYPE_DOWN] | toKey[at_c<4>(_val) = ast::ForType::TYPE_TO])
				> expressionGrammar[at_c<2>(_val) = _1]
				> doKey
				> statementRule[at_c<3>(_val) = _1]
			;
		beginKey = no_case["begin"];
		endKey = no_case["end"];
 		compoundStmtRule = beginKey > -statementListRule  > endKey ;

		complexStmtRule = 
			compoundStmtRule
			| ifStmlRule
 			| whileStmlRule
 			| forStmlRule
			;

		statementRule =
				
 			 inputStmtRule
			| variableStmtRule	
			| complexStmtRule
			| out

			
			| no_case["break"] > semicolonSign
			| semicolonSign
 			| eol
// 			| commentGrammar
			;

		statementListRule = +(statementRule - endKey);


		
// 		on_error<qi::fail>(expressionGrammar.identifierRule,
// 			FunctionErrorGrammarHandler(errorHandler)(
// 				"Error! Expecting ", _4, _3));
		statementListRule.name("statementListRule");
		out.name("out");
		statementRule.name("statementRule");
		compoundStmtRule.name("compoundStmtRule");
		ifStmlRule.name("ifStmlRule");
		inputSubStmt.name("input");
		inputParamList.name("inputList");
		ifKey.name("if");
		thenKey.name("then");
		elseKey.name("else");
		beginKey.name("begin");
		endKey.name("end");
		doKey.name("do");
		downToKey.name("downto");
		toKey.name("to");
		whileKey.name("while");
		forKey.name("for");
		commaSign.name(",");
		semicolonSign.name(";");
		equalSign.name("=");
		variableValue.name("value");
		leftBracket.name("(");
		rightBracket.name(")");

		thickKey.name("thick");
		thickNum.name("thickNum");

		preciseKey.name("precise");
		preciseNum.name("preciseNum");

		layerKey.name("layer");
		layerNum.name("layerNum");

		alignKey.name("align");
		alignNum.name("alignNum");

		valignNum.name("valign");
		valignNum.name("valignNum");
		
		moveKey.name("move");
		moveNum.name("moveNum");

// 		on_error<qi::fail>(semicolon,
// 			FunctionErrorGrammarHandler(errorHandler)(
// 				"Error! Expecting ", _4, _3));
		on_error<qi::fail>(statementListRule,
			FunctionErrorGrammarHandler(errorHandler)(
				"Error! Expecting ", _4, _3));

	}

	virtual ~StatementGrammar() {

	}
protected:
	qi::symbols<char, uint8_t> stmt;
	ColorSymbols colorSymbols;
	LineTypeSymbols lineTypeSymbols;

	
	Rule start;
	Rule ifKey, thenKey, elseKey, beginKey, endKey, doKey, downToKey, toKey, whileKey, forKey;
	Rule commaSign, semicolonSign, equalSign, leftBracket, rightBracket;

	qi::rule<Iterator, string(), SkipperGrammar<Iterator>> thickNum, preciseNum, layerNum, alignNum, valignNum, moveNum, hideNum;
	qi::rule<Iterator, string(), SkipperGrammar<Iterator>> thickKey, preciseKey, layerKey, alignKey, valignKey, moveKey, hideKey;
	
	qi::rule<Iterator, ast::Statement(), SkipperGrammar<Iterator>> statementRule;
	qi::rule<Iterator, ast::StatementList(), SkipperGrammar<Iterator>> statementListRule;
	//input ”Ôæ‰
	qi::rule<Iterator, list<double>(), SkipperGrammar<Iterator>> inputParamList;
	qi::rule<Iterator, ast::InputStmt(), SkipperGrammar<Iterator>> inputSubStmt;
	qi::rule<Iterator, ast::InputStmtList(), SkipperGrammar<Iterator>> inputStmtRule;
	//Rule inputStmtRule, inputParamList, inputSubStmt;
	//variable ”Ôæ‰
	qi::rule<Iterator, ast::BaseOperand(), SkipperGrammar<Iterator>> variableValue;
	qi::rule<Iterator, ast::VariableStmt(), SkipperGrammar<Iterator>> variableSubStmt;
	qi::rule<Iterator, ast::VariableStmtList(), SkipperGrammar<Iterator>> variableStmtRule;
	//Rule variableStmtRule, variableSubStmt;
	//∏≥÷µ”Ôæ‰∫Õ ‰≥ˆ”Ôæ‰
// 	qi::rule<Iterator, ast::Assignment(), SkipperGrammar<Iterator>> assignment;
	qi::rule<Iterator, ast::Out(), SkipperGrammar<Iterator>> out;
	//Rule assignmentAndOutRule;

	
	
	
	qi::rule<Iterator, ast::ColorModifer(), SkipperGrammar<Iterator>> colorModifer;
	qi::rule<Iterator, ast::TypeModifer(), SkipperGrammar<Iterator>> thickModifer, layerModifer, preciseModifer;
	qi::rule<Iterator, ast::TypeModifer(), SkipperGrammar<Iterator>> alignModifer,valignModifer, hideModifer, moveModifer;
	qi::rule<Iterator, ast::ModiferStmtOperand(), SkipperGrammar<Iterator>> modiferStmt;
	qi::rule<Iterator, ast::ModiferStmtOperandList(), SkipperGrammar<Iterator>> modiferList;
// 	Rule modiferList, modiferStmt;

// 	Rule forStmlRule, whileStmlRule;
	qi::rule<Iterator, ast::ForStatement(), SkipperGrammar<Iterator>> forStmlRule;
	qi::rule<Iterator, ast::WhileStatement(), SkipperGrammar<Iterator>> whileStmlRule;
// 	Rule ifStmlRule, ifPartRule, thenRule;
// 	qi::rule<Iterator, ast::Statement(), SkipperGrammar<Iterator>>  thenRule;
// 	qi::rule<Iterator, ast::ThenStatement(), SkipperGrammar<Iterator>> ifPartRule;
	qi::rule<Iterator, ast::IfStatement(), SkipperGrammar<Iterator>> ifStmlRule;

	//∏¥∫œ”Ôæ‰
	qi::rule<Iterator, ast::CompoundStatement(), SkipperGrammar<Iterator>> compoundStmtRule;
	//∏¥‘””Ôæ‰
	qi::rule<Iterator, ast::ComplexStatement(), SkipperGrammar<Iterator>> complexStmtRule;



	ExpressionGrammar<Iterator> expressionGrammar;
	SkipperGrammar<Iterator> commentGrammar;

	

};