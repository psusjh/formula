#pragma once
#include <string>
#include <vector>

using namespace std;

#include "OpratorSymbols.h"
#include "ModifierSymbols.h"

#include "SkipperGrammar.hpp"
#include "CodeCompiler.h"
#include "ast.hpp"

template<typename Iterator>
class ExpressionGrammar: public qi::grammar<Iterator, ast::Expression(), ascii::space_type>
{
	typedef qi::rule<Iterator, ascii::space_type> Rule;
public:

	ExpressionGrammar() :
		ExpressionGrammar::base_type(expr, "ExpressionGrammar")
	{
		using qi::char_;
		using qi::double_;
		using qi::lexeme;
		using qi::lit;
		using qi::_1;

		using qi::ascii::no_case;

		addIndicator("ma");

		keyWordsSymbols.add("if")("else")("then")
			("for")("to")("downto")("break")
			("do")("while")
			("begin")("end")
			("and")("or")
			("varible")
			("input")
			;
	
  		expr = logicalOrExpr.alias();
		logicalOrExpr = logicalAndExpr >> *(no_case[logicalOrOpSymbols] > logicalAndExpr);
		logicalAndExpr = bitOrExpr >> *(no_case[logicalAndOpSymbols] > bitOrExpr);
		bitOrExpr = bitXorExpr >> *(bitOrOpSymbols > bitXorExpr);
		bitXorExpr = bitAndExpr >> *(bitXorOpSymbols > bitAndExpr);
		bitAndExpr = equalityExpr >> *(bitAndOpSymbols > equalityExpr);
		equalityExpr = relationalExpr >> *(equlityOpSymbols > relationalExpr);
		relationalExpr = additiveExpr >> *(relationalOpSymbols > additiveExpr);
		additiveExpr = multiExpr >> *(additiveOpSymbols > multiExpr);
 		multiExpr = unaryExpr >> *(multiOpSymbols > unaryExpr);
		unaryExpr = mainExpr
			| (unaryOpSymbols > unaryExpr)
			;

		wordRule %= lexeme[(char_("a-zA-Z\x80-\xff_")) >> *(char_("a-zA-Z0-9\x80-\xff_"))];
		identifierRule %= !(lexeme[no_case[keyWordsSymbols]] | lexeme[no_case[indicatorSymbols]]) >> wordRule;

 		argumentExpr = -(expr% ',');

		functionExpr =
			indicatorSymbols > -('(' > argumentExpr > ')')
			| indicatorSymbols
			;

		stringRule %= char_('\'') >> *(lit("\\\'") | char_ - char_("\'")) >> char_('\'');

		quoteRule %= char_('\"') >> *(lit("\\\"") | char_ - char_("\"")) >> char_('\"');

		mainExpr =
			double_
			| stringRule
 			| quoteRule
 			| identifierRule
 			| functionExpr
 			| '(' > expr > ')'
			;
	}

	~ExpressionGrammar() {

	}
public:
	void addKeyWords(const string& keyWords) {
		this->keyWordsSymbols.add(keyWords);
	}
	void addIndicator(const string& indicator) {
		this->indicatorSymbols.add(indicator, indicator);		
	}
	
	bool hasIndicator(const string& indicator) {
		return this->indicatorSymbols.find(indicator) != nullptr;
	}
public:
	qi::rule<Iterator, string(), ascii::space_type> identifierRule, stringRule;
protected:
	qi::rule<Iterator, ast::Expression(), ascii::space_type> expr;

	qi::rule<Iterator, string(), ascii::space_type> wordRule, quoteRule;

	qi::rule<Iterator, ast::CalcOperand(), ascii::space_type> unaryExpr, mainExpr;
	qi::rule<Iterator, ast::Expression(), ascii::space_type> multiExpr, additiveExpr, relationalExpr, equalityExpr;
	qi::rule<Iterator, ast::Expression(), ascii::space_type> bitAndExpr, bitXorExpr, bitOrExpr;
	qi::rule<Iterator, ast::Expression(), ascii::space_type> logicalAndExpr, logicalOrExpr;

	qi::rule<Iterator, ast::FunctionCall(), ascii::space_type> functionExpr;
	qi::rule<Iterator, vector<ast::Expression>(), ascii::space_type> argumentExpr;


	qi::symbols<char>			keyWordsSymbols;
	qi::symbols<char,string>			indicatorSymbols;
	UnaryOpSymbols				unaryOpSymbols;
	MultiplicativeOpSymbols		multiOpSymbols;
	AdditiveOpSymbols			additiveOpSymbols;
	RelationalOpSymbols			relationalOpSymbols;
	EqulityOpSymbols			equlityOpSymbols;
	BitAndOpSymbols				bitAndOpSymbols;
	BitXOROpSymbols				bitXorOpSymbols;
	BitOrOpSymbols				bitOrOpSymbols;
	LogicalOrOpSymbols			logicalOrOpSymbols;
	LogicalAndOpSymbols			logicalAndOpSymbols;

	

};



