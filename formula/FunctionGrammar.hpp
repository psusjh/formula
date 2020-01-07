#pragma once
struct Instruct
{
	uint16_t id;
	char name[16];
	uint8_t instructAttr;
	uint8_t paramNum;
	char nameEx[32];
	char usage[128];
};


#include "SkipperGrammar.hpp"

template<typename Iterator>
class FunctionGrammar : public qi::grammar<Iterator, ascii::space_type> {
	typedef qi::rule<Iterator, ascii::space_type> Rule;
public:
	FunctionGrammar():
		FunctionGrammar::base_type(start, "FunctionGrammar")
	{

	}

	virtual ~FunctionGrammar() {

	}
public:
	Rule start;

};