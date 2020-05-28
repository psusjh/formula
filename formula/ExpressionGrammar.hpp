#pragma once
#include <string>
#include <vector>

using namespace std;

#include "OpratorSymbols.h"
#include "ModifierSymbols.h"
#include "ErrorGrammarHandler.hpp"
#include "SkipperGrammar.hpp"
#include "Code.h"
#include "ast.hpp"

#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/bool.hpp>


template<typename Iterator>
struct Success {
	template <typename, typename>
	struct result { typedef void type; };
	void operator()(ast::CalcOperand& r, Iterator pos)const {
		if (r.type() == typeid(ast::Nil)) {
			cout << "Nil\n";
		}
		else if (r.type() == typeid(uint32_t)) {
			cout << "Success\tuint32_t\t";
			uint32_t index = boost::get<uint32_t>(r);
			cout << "index:" << index;
			if (index < sizeof(KeyWord) / sizeof(char*)) {
				cout << "\tKeyWord:" << KeyWord[index];
			}
			cout << endl;
		}
		else if (r.type() == typeid(string)) {
			cout << "Success\tstring:" << boost::get<string>(r) << "\n";
		}
		else {
			cout << "Success\ttype:" << r.type().name() << endl;
		}
	}
	
};

template<typename Iterator>
class ExpressionGrammar: public qi::grammar<Iterator, ast::Expression(), SkipperGrammar<Iterator>>
{
	
	typedef qi::rule<Iterator, SkipperGrammar<Iterator>> Rule;
public:

	ExpressionGrammar(ErrorGrammarHandler<Iterator>& errorHandler) :
		ExpressionGrammar::base_type(expr, "ExpressionGrammar")
	{
		using boost::phoenix::function;

		typedef function< ErrorGrammarHandler<Iterator> > FunctionErrorGrammarHandler;

		typedef function<Success<Iterator>> FunctionSuccessHandler;

		using qi::char_;
		using qi::double_;
		using qi::lexeme;
		using qi::lit;
		using qi::on_error;
		using qi::on_success;
		using qi::raw;
		using qi::alnum;
		using qi::_val;

		qi::_1_type _1;
		qi::_2_type _2;
		qi::_3_type _3;
		qi::_4_type _4;

		using qi::ascii::no_case;

		addIndicator("c", SFI_C);
// 		addIndicator("if", SFI_IF);
		addIndicator("ma", SFI_MA);

		for (size_t i = 0; i < sizeof(KeyWord) / sizeof(char*); ++i) {
			keyWordsSymbols.add(KeyWord[i], i);
		}
		
	
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

		wordRule = raw[lexeme[(char_("a-zA-Z\x80-\xff_")) >> *(char_("a-zA-Z0-9\x80-\xff_"))]];
		key = ((lexeme[no_case[indicatorSymbols] | no_case[keyWordsSymbols]]) >> !(alnum | '_'));

		identifierRule = !key
			>> wordRule;

 		argumentExpr = -(expr % ',');

		functionExpr =
			no_case[indicatorSymbols] >> -('(' > argumentExpr > ')')
			/*| indicatorSymbols*/
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
		identifierRule.name("identifierRule");
		functionExpr.name("functionExpr");
		wordRule.name("wordRule");
		mainExpr.name("mainExpr");
		keyWordsSymbols.name("keyWordsSymbols");
		indicatorSymbols.name("indicatorSymbols");
		key.name("key");
		on_success(key, FunctionSuccessHandler()(_val, _1));

		on_error<qi::fail>(mainExpr,
			FunctionErrorGrammarHandler(errorHandler)(
				"Error! Expecting ", _4, _3));
// 		on_error<qi::fail>(functionExpr,
// 			FunctionErrorGrammarHandler(errorHandler)(
// 				"Error! Expecting ", _4, _3));
	}

	~ExpressionGrammar() {

	}
public:
	void addKeyWords(const string& keyWords) {
		this->keyWordsSymbols.add(keyWords);
	}
	void addIndicator(const string& indicator, SFI sfi) {
		this->indicatorSymbols.add(indicator, sfi);		
	}
	
	bool hasIndicator(const string& indicator) {
		return this->indicatorSymbols.find(indicator) != nullptr;
	}
public:
	qi::rule<Iterator, ast::Iditenfier(), SkipperGrammar<Iterator>> identifierRule;
	qi::rule<Iterator, string(), SkipperGrammar<Iterator>> stringRule;
protected:
	qi::rule<Iterator, ast::CalcOperand(), SkipperGrammar<Iterator>> key;

	qi::rule<Iterator, ast::Expression(), SkipperGrammar<Iterator>> expr;

	qi::rule<Iterator, string(), SkipperGrammar<Iterator>> wordRule;

	qi::rule<Iterator, ast::QuoteString(), SkipperGrammar<Iterator>> quoteRule;

	qi::rule<Iterator, ast::CalcOperand(), SkipperGrammar<Iterator>> unaryExpr, mainExpr;
	qi::rule<Iterator, ast::Expression(), SkipperGrammar<Iterator>> multiExpr, additiveExpr, relationalExpr, equalityExpr;
	qi::rule<Iterator, ast::Expression(), SkipperGrammar<Iterator>> bitAndExpr, bitXorExpr, bitOrExpr;
	qi::rule<Iterator, ast::Expression(), SkipperGrammar<Iterator>> logicalAndExpr, logicalOrExpr;

	qi::rule<Iterator, ast::FunctionCall(), SkipperGrammar<Iterator>> functionExpr;
	qi::rule<Iterator, ast::ExpressionList(), SkipperGrammar<Iterator>> argumentExpr;


	qi::symbols<char, uint32_t>			keyWordsSymbols;
	qi::symbols<char, SFI>				indicatorSymbols;
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



