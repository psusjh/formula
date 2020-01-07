#pragma once

#include <boost/spirit/include/qi.hpp>


namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template<typename Iterator>
class SkipperGrammar:public qi::grammar<Iterator, ascii::space_type>
{
	typedef qi::rule<Iterator, ascii::space_type> Rule;
public:

	SkipperGrammar() :
		SkipperGrammar::base_type(start, "SkipperGrammar")
	{
		using qi::eol;
		using qi::eoi;

		qi::char_type char_;

		start =
			
			"/*" >> *(char_ - "*/") > "*/"     // C-style comments
			|'{'>>*(char_- '}') >'}'
			|"//">>*(char_ - eol)>(eol|eoi)
			;
	}

	~SkipperGrammar() {

	}
public:
	Rule start;
};


