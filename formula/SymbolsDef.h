#pragma once

#include <boost/spirit/include/qi.hpp>
namespace qi = boost::spirit::qi;


#define DEFINE_SYMBOLS_START(NAME, T) struct NAME:qi::symbols<char, T>{ NAME() {
#define ADD(op, value) add(op, value);
#define DEFINE_SYMBOLS_END }};


