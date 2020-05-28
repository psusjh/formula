#pragma once

#include <boost/spirit/include/qi.hpp>
#include "InternalFunction.h"
namespace qi = boost::spirit::qi;


#define DEFINE_SYMBOLS_START(NAME, T) struct NAME:qi::symbols<char, T>{ NAME() {
#define ADD_REG(op, value, num, cn, usage) add(op, value);\
InternalFunctionSingleton::instance().registerFunction(value, op, 0, num, cn, usage);

#define ADD(op, value) add(op, value);

#define DEFINE_SYMBOLS_END }};


