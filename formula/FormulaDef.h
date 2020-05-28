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

	SFI_CONTROL = 1,			//����
	SFI_JUMP,					//��ת
	SFI_LOOP,					//ѭ��
	SFI_JEQ,					//�����ת
	SFI_JNEQ,					//�������ת
	SFI_JGT,					//������ת
	SFI_JGE,					//���ڵ�����ת
	SFI_JLT,					//С����ת
	SFI_JLE,					//С�ڵ�����ת

	SFI_OPERATOR = 20,			//������
	SFI_POSITIVE,				//����
	SFI_NEGATIVE,				//����
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

	SFI_DATA_S = 80,			//��������
	SFI_O, SFI_OPEN, SFI_H, SFI_HIGH, SFI_L, SFI_LOW, SFI_C, SFI_CLOSE,


	SFI_REFERENCE_FUNC,			//���ú���
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
* ���࣬@sfi ���࿪ʼ��@name ��������
* ��������Ǻ�һ������Ŀ�ʼ����������Ҫ˳����
*/
const SfiCatalog sfiCatalog[] = {
	{SFI_CONTROL,			"���Ʒ�"},	
	{SFI_OPERATOR,			"������"},
	{SFI_DATA_S,			"�������к���"},
	{SFI_REFERENCE_FUNC,	"���ú���"}
};



#endif