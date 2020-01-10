#pragma once


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

	SFI_OPERATOR = 20,			//
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
	SFI_OR,						//or

	SFI_EQU,					//=				
	SFI_NOT_EQU,				//<>,!=

	SFI_LESS,					//<
	SFI_LESS_EQU,				//<=
	SFI_GREATER,				//>
	SFI_GREATER_EQU,			//>=

	SFI_ASSIGN,					//:=
	SFI_OUT,					//:

	SFI_DATA_S = 80,
	SFI_O, SFI_OPEN, SFI_H, SFI_HIGH, SFI_L, SFI_LOW, SFI_C, SFI_CLOSE,



	SFI_MA,
	SFI_MACD


};
