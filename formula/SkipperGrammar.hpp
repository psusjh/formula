#pragma once

#include <boost/spirit/include/qi.hpp>


namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template<typename Iterator>
class SkipperGrammar:public qi::grammar<Iterator/*, ascii::space_type*/>
{
	typedef qi::rule<Iterator> Rule;
public:

	SkipperGrammar() :
		SkipperGrammar::base_type(start, "SkipperGrammar")
	{
		using qi::eol;
		using qi::eoi;
		using qi::eps;
		using qi::lit;
		using qi::blank;
		qi::char_type char_;
		ascii::space_type space;
		start =
			space
			|"/*" >> *(char_ - "*/") > "*/"     // C-style comments
 			| lit("//") >> *(char_ - eol) > eol
			;
	}

	~SkipperGrammar() {

	}
public:
	Rule start;
};


