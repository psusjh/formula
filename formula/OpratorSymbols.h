#pragma once
#include "FormulaDef.h"
#include "SymbolsDef.h"



//一元操作符
DEFINE_SYMBOLS_START(UnaryOpSymbols, SFI)
	ADD_REG("+", SFI_POSITIVE, 1, "正号", "例如：+x")
	ADD_REG("-", SFI_NEGATIVE, 1, "负号", "例如：-x")
	ADD_REG("!", SFI_NOT, 1, "否", "例如：!x")
	ADD_REG("~", SFI_BIT_NOT, 1, "位否", "例如：~x 该操作将x转换为long类型")
DEFINE_SYMBOLS_END

//加减操作符
DEFINE_SYMBOLS_START(AdditiveOpSymbols, SFI)
	ADD_REG("+", SFI_PLUS, 2, "加", "例如：x+y")
	ADD_REG("-", SFI_MINUS, 2, "减", "例如：x-y")
DEFINE_SYMBOLS_END

//乘除取余操作符
DEFINE_SYMBOLS_START(MultiplicativeOpSymbols, SFI)
	ADD_REG("*", SFI_MUL, 2, "乘", "例如：x*y")
	ADD_REG("/", SFI_DIV, 2, "除", "例如：x/y")
	ADD_REG("%", SFI_MOD, 2, "取模", "例如：x%y 该操作将x,y转换为long类型")
DEFINE_SYMBOLS_END


DEFINE_SYMBOLS_START(RelationalOpSymbols, SFI)
	ADD_REG("<", SFI_LESS, 2, "小于", "例如：x<y")
	ADD_REG("<=", SFI_LESS_EQU, 2, "小于等于", "例如：x<=y")
	ADD_REG(">", SFI_GREATER, 2, "大于", "例如：x>y")
	ADD_REG(">=", SFI_GREATER_EQU,2, "大于等于", "例如：x>=y")
DEFINE_SYMBOLS_END

DEFINE_SYMBOLS_START(BitAndOpSymbols, SFI)
	ADD_REG("&", SFI_BIT_AND, 2, "位与", "例如：x&y, 该操作将x,y转换为long类型")
DEFINE_SYMBOLS_END

DEFINE_SYMBOLS_START(BitXOROpSymbols, SFI)
	ADD_REG("^", SFI_BIT_XOR, 2, "位异或", "例如：x^y, 该操作将x,y转换为long类型")
DEFINE_SYMBOLS_END

DEFINE_SYMBOLS_START(BitOrOpSymbols, SFI)
	ADD_REG("|", SFI_BIT_OR, 2, "位或", "例如：x|y, 该操作将x,y转换为long类型")
DEFINE_SYMBOLS_END

DEFINE_SYMBOLS_START(EqulityOpSymbols, SFI)
	ADD_REG("=", SFI_EQU, 2, "等于", "例如：x=y, 返回 0，1")
	ADD_REG("<>", SFI_NOT_EQU, 2, "不等于", "例如：x<>y, 返回 0，1")
	ADD_REG("!=", SFI_NOT_EQU_N, 2, "不等于", "例如：x<>y, 返回 0，1")
DEFINE_SYMBOLS_END

DEFINE_SYMBOLS_START(LogicalOrOpSymbols, SFI)
	ADD_REG("||", SFI_OR_V, 2, "逻辑或", "例如：x || y, 返回 0，1")
	ADD_REG("or", SFI_OR, 2, "逻辑或", "例如：x or y, 返回 0，1")
DEFINE_SYMBOLS_END


DEFINE_SYMBOLS_START(LogicalAndOpSymbols, SFI)
	ADD_REG("&&", SFI_AND_A, 2, "逻辑与", "例如：x && y, 返回 0，1")
	ADD_REG("and", SFI_AND, 2, "逻辑与", "例如：x and y, 返回 0，1")
DEFINE_SYMBOLS_END