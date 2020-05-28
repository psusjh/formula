#pragma once
#ifndef __formula__def__h__
#define __formula__def__h__

#ifdef _WIN32
#define stricmp _stricmp
#else
#define stricmp strcasecmp
#endif

#define RGB2INT(r,g,b)          ((uint32_t)(((uint8_t)(r)|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)(uint8_t)(b))<<16)))

enum SFI {
	SFI_VOID,

	SFI_CONTROL = 1,			//控制
	SFI_JUMP,					//跳转
	SFI_LOOP,					//循环
	SFI_JEQ,					//相等跳转
	SFI_JNEQ,					//不相等跳转
	SFI_JGT,					//大于跳转
	SFI_JGE,					//大于等于跳转
	SFI_JLT,					//小于跳转
	SFI_JLE,					//小于等于跳转

	SFI_OPERATOR = 20,			//操作符
	SFI_POSITIVE,				//正号
	SFI_NEGATIVE,				//负号
	SFI_NOT,					//!
	SFI_PLUS,					//+
	SFI_MINUS,					//-
	SFI_MUL,					//*
	SFI_DIV,					///
	SFI_MOD,					//%
	SFI_BIT_AND,				//&
	SFI_BIT_OR,					//|
	SFI_BIT_XOR,				//^
	SFI_BIT_NOT,				//~

	SFI_AND,					//and
	SFI_AND_A,					//&&
	SFI_OR,						//or
	SFI_OR_V,					//||

	SFI_EQU,					//=				
	SFI_NOT_EQU,				//<>
	SFI_NOT_EQU_N,				//!=

	SFI_LESS,					//<
	SFI_LESS_EQU,				//<=
	SFI_GREATER,				//>
	SFI_GREATER_EQU,			//>=

	SFI_ASSIGN,					//:=
	SFI_OUT,					//:

	SFI_DATA_S = 80,			//行情序列
	SFI_O, SFI_OPEN, SFI_H, SFI_HIGH, SFI_L, SFI_LOW, SFI_C, SFI_CLOSE,


	SFI_REFERENCE_FUNC,			//引用函数
	SFI_MA,
	SFI_IF
};

const char* const KeyWord[] = {
	"if",
	"else",
	"then",
	"for",
	"to",
	"downto",
	"break",
	"do",
	"while",
	"begin",
	"end",
	"and",
	"or",
	"variable",
	"input"
};

struct SfiCatalog {
	SFI sfi;
	const char* name;
};
/**
* 分类，@sfi 分类开始，@name 分类名字
* 分类结束是后一个分类的开始，所以数组要顺序存放
*/
const SfiCatalog sfiCatalog[] = {
	{SFI_CONTROL,			"控制符"},	
	{SFI_OPERATOR,			"操作符"},
	{SFI_DATA_S,			"行情序列函数"},
	{SFI_REFERENCE_FUNC,	"引用函数"}
};



#endif