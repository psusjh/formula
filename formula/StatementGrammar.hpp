#pragma once


#include <boost/spirit/include/phoenix.hpp>


#include "SkipperGrammar.hpp"
#include "ExpressionGrammar.hpp"
#include "ErrorGrammarHandler.hpp"
#include "ModifierSymbols.h"
#include "Code.h"
#include "ast.hpp"


template<typename Iterator>
class StatementGrammar : public qi::grammar<Iterator, ast::StatementList(), ascii::space_type>
{
	typedef qi::rule<Iterator,  ascii::space_type> Rule;	
public:
	StatementGrammar(ErrorGrammarHandler<Iterator>& errorHandler)
		:StatementGrammar::base_type(statementListRule, "StatementGrammar")
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

		using qi::on_error;
		using qi::as_string;
		using qi::no_case;
		using qi::repeat;
		using qi::lexeme;
		using boost::phoenix::at_c;
		using boost::phoenix::push_back;


		expressionGrammar.addIndicator("ma");
		expressionGrammar.addIndicator("macd");
		expressionGrammar.addIndicator("kdj");

		
		colorModifer %= lexeme[
			no_case["color"]
				>> (no_case[colorSymbols] | repeat(6)[no_case[char_("0-9a-f")]])
		];
		thickModifer = lexeme[no_case["linethick"][at_c<0>(_val) = "linethick"] >> char_("0-7")[at_c<1>(_val) += _1]];
		layerModifer = lexeme[no_case["layer"][at_c<0>(_val) = "layer"] >> char_("0-7")[at_c<1>(_val) += _1]];
		preciseModifer = lexeme[no_case["precise"][at_c<0>(_val) = "precise"] >> char_("0-7")[at_c<1>(_val) += _1]];
		alignModifer = lexeme[no_case["align"][at_c<0>(_val) = "align"] >> char_("0-2")[at_c<1>(_val) += _1]];
		valignModifer = lexeme[no_case["valign"][at_c<0>(_val) = "valign"] >> char_("0-2")[at_c<1>(_val) += _1]];
		lineModifer = lexeme[no_case["hideval"][at_c<0>(_val) = "hideval"]];
		moveModifer = lexeme[no_case["move"][at_c<0>(_val) = "move"] >> +char_("0-9")[at_c<1>(_val) += _1]];

		modiferStmt %= colorModifer
			| thickModifer
			| layerModifer
			| preciseModifer
			| alignModifer
			| valignModifer
			| lineModifer
			| moveModifer
			| lineTypeSymbols
			;

		modiferList %= -(modiferStmt % char_(','));

		inputParamList %= -(double_ % char_(','));
 		inputSubStmt %= expressionGrammar.identifierRule >> '(' >> inputParamList >> ')';
 		inputStmtRule %= no_case["input:"] >> -(inputSubStmt % char_(',')) >> ';';

		variableSubStmt = expressionGrammar.identifierRule >> '=' >> (double_ | expressionGrammar.stringRule);
		variableStmtRule = no_case["variable:"] >> -(variableSubStmt % ',') >> ';';

		out = expressionGrammar.identifierRule[at_c<1>(_val) = _1] >(
			(lit(":=")[at_c<0>(_val) = "a"] > expressionGrammar[at_c<2>(_val) = _1] >> ';')
			|(lit(":")[at_c<0>(_val) = "o"] > expressionGrammar[at_c<2>(_val) = _1] > -(',' > modiferList)[at_c<3>(_val) = _1] >> ';')
			)
			;

		
		thenRule = no_case["then"] > statementRule;
		ifPartRule = thenRule > -(no_case["else"] > statementRule);
		ifStmlRule = no_case["if"] > expressionGrammar > ifPartRule;

 		whileStmlRule = no_case["while"] > expressionGrammar > no_case["do"] > statementRule;
 
		forStmlRule = no_case["for"]
				> expressionGrammar.identifierRule[at_c<0>(_val) = _1]
				> '='
				> expressionGrammar[at_c<1>(_val) = _1]
				> (no_case["downto"][at_c<4>(_val) = "d"] | no_case["to"][at_c<4>(_val) = "t"])
				> expressionGrammar[at_c<2>(_val) = _1]
				> no_case["do"]
				> statementRule[at_c<3>(_val) = _1]
			;

 		compoundStmtRule = no_case["begin"] > statementListRule > no_case["end"];

		complexStmtRule = ifStmlRule
 			| whileStmlRule
 			| forStmlRule
 			| compoundStmtRule
			;

		statementRule =
			commentGrammar	
 			| inputStmtRule
			| variableStmtRule	
			| out
// 			| assignment 
			| complexStmtRule
			| no_case["break"] > ";"
			| lit(";")
			| eol
			;

		statementListRule = +statementRule;


		
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
	qi::rule<Iterator, ast::Statement(), ascii::space_type> statementRule;
	qi::rule<Iterator, ast::StatementList(), ascii::space_type> statementListRule;
	//input ”Ôæ‰
	qi::rule<Iterator, list<double>(), ascii::space_type> inputParamList;
	qi::rule<Iterator, ast::InputStmt(), ascii::space_type> inputSubStmt;
	qi::rule<Iterator, ast::InputStmtList(), ascii::space_type> inputStmtRule;
	//Rule inputStmtRule, inputParamList, inputSubStmt;
	//variable ”Ôæ‰
	qi::rule<Iterator, ast::VariableStmt(), ascii::space_type> variableSubStmt;
	qi::rule<Iterator, ast::VariableStmtList(), ascii::space_type> variableStmtRule;
	//Rule variableStmtRule, variableSubStmt;
	//∏≥÷µ”Ôæ‰∫Õ ‰≥ˆ”Ôæ‰
// 	qi::rule<Iterator, ast::Assignment(), ascii::space_type> assignment;
	qi::rule<Iterator, ast::Out(), ascii::space_type> out;
	//Rule assignmentAndOutRule;

	
	
	
	qi::rule<Iterator, ast::ColorModifer(), ascii::space_type> colorModifer;
	qi::rule<Iterator, ast::TypeModifer(), ascii::space_type> thickModifer, layerModifer, preciseModifer;
	qi::rule<Iterator, ast::TypeModifer(), ascii::space_type> alignModifer,valignModifer, lineModifer, moveModifer;
	qi::rule<Iterator, ast::ModiferStmtOperand(), ascii::space_type> modiferStmt;
	qi::rule<Iterator, ast::ModiferStmtOperandList(), ascii::space_type> modiferList;
// 	Rule modiferList, modiferStmt;

// 	Rule forStmlRule, whileStmlRule;
	qi::rule<Iterator, ast::ForStatement(), ascii::space_type> forStmlRule;
	qi::rule<Iterator, ast::WhileStatement(), ascii::space_type> whileStmlRule;
// 	Rule ifStmlRule, ifPartRule, thenRule;
	qi::rule<Iterator, ast::Statement(), ascii::space_type> ifPartRule, thenRule;
	qi::rule<Iterator, ast::IfStatement(), ascii::space_type> ifStmlRule;

	//∏¥∫œ”Ôæ‰
	qi::rule<Iterator, ast::StatementList(), ascii::space_type> compoundStmtRule;
	//∏¥‘””Ôæ‰
	qi::rule<Iterator, ast::ComplexStatement(), ascii::space_type> complexStmtRule;



	ExpressionGrammar<Iterator> expressionGrammar;
	SkipperGrammar<Iterator> commentGrammar;

	

};