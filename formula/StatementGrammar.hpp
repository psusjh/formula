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
		using boost::phoenix::at_c;
		using boost::phoenix::push_back;
		using qi::matches;


		
		

		
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

		inputParamList %= +(double_ % char_(','));
 		inputSubStmt %= expressionGrammar.identifierRule >> '(' >> inputParamList >> ')';
 		inputStmtRule %= no_case["input:"] >> -(inputSubStmt % char_(',')) >> ';';

		variableSubStmt = expressionGrammar.identifierRule >> '=' >> (double_ | expressionGrammar.stringRule);
		variableStmtRule = no_case["variable:"] >> -(variableSubStmt % ',') >> ';';

		out = expressionGrammar.identifierRule[at_c<1>(_val) = _1] >(
			(lit(":=")[at_c<0>(_val) = ast::StatementType::assignment] > expressionGrammar[at_c<2>(_val) = _1] >> ';')
			|(lit(":")[at_c<0>(_val) = ast::StatementType::out] > expressionGrammar[at_c<2>(_val) = _1] > -(',' > modiferList)[at_c<3>(_val) = _1] >> ';')
			)
			;

		
// 		thenRule = no_case["then"] > statementRule;
// 		ifPartRule = (thenRule) > -(no_case["else"] > statementRule);
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
				> '='
				> expressionGrammar[at_c<1>(_val) = _1]
				> (downToKey[at_c<4>(_val) = ast::ForType::TYPE_DOWN] | toKey[at_c<4>(_val) = ast::ForType::TYPE_TO])
				> expressionGrammar[at_c<2>(_val) = _1]
				> doKey
				> statementRule[at_c<3>(_val) = _1]
			;
		begin = no_case["begin"];
		end = no_case["end"];
 		compoundStmtRule = begin > -statementListRule  > end ;

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
// 			| assignment 
			
			| no_case["break"] > ";"
			| lit(";")
 			| eol
// 			| commentGrammar
			;

		statementListRule = +(statementRule - end);


		
// 		on_error<qi::fail>(expressionGrammar.identifierRule,
// 			FunctionErrorGrammarHandler(errorHandler)(
// 				"Error! Expecting ", _4, _3));
		statementListRule.name("statementListRule");
		out.name("out");
		statementRule.name("statementRule");
		compoundStmtRule.name("compoundStmtRule");
		ifStmlRule.name("ifStmlRule");
		
		ifKey.name("if");
		thenKey.name("then");
		elseKey.name("else");
		begin.name("begin");
		end.name("end");
		doKey.name("do");
		downToKey.name("downto");
		toKey.name("to");
		whileKey.name("while");
		forKey.name("for");


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
	Rule ifKey, thenKey, elseKey, begin, end, doKey, downToKey, toKey, whileKey, forKey;
	qi::rule<Iterator, ast::Statement(), SkipperGrammar<Iterator>> statementRule;
	qi::rule<Iterator, ast::StatementList(), SkipperGrammar<Iterator>> statementListRule;
	//input ”Ôæ‰
	qi::rule<Iterator, list<double>(), SkipperGrammar<Iterator>> inputParamList;
	qi::rule<Iterator, ast::InputStmt(), SkipperGrammar<Iterator>> inputSubStmt;
	qi::rule<Iterator, ast::InputStmtList(), SkipperGrammar<Iterator>> inputStmtRule;
	//Rule inputStmtRule, inputParamList, inputSubStmt;
	//variable ”Ôæ‰
	qi::rule<Iterator, ast::VariableStmt(), SkipperGrammar<Iterator>> variableSubStmt;
	qi::rule<Iterator, ast::VariableStmtList(), SkipperGrammar<Iterator>> variableStmtRule;
	//Rule variableStmtRule, variableSubStmt;
	//∏≥÷µ”Ôæ‰∫Õ ‰≥ˆ”Ôæ‰
// 	qi::rule<Iterator, ast::Assignment(), SkipperGrammar<Iterator>> assignment;
	qi::rule<Iterator, ast::Out(), SkipperGrammar<Iterator>> out;
	//Rule assignmentAndOutRule;

	
	
	
	qi::rule<Iterator, ast::ColorModifer(), SkipperGrammar<Iterator>> colorModifer;
	qi::rule<Iterator, ast::TypeModifer(), SkipperGrammar<Iterator>> thickModifer, layerModifer, preciseModifer;
	qi::rule<Iterator, ast::TypeModifer(), SkipperGrammar<Iterator>> alignModifer,valignModifer, lineModifer, moveModifer;
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