#pragma once
#include "FormulaDef.h"
#include "SymbolsDef.h"



//һԪ������
DEFINE_SYMBOLS_START(UnaryOpSymbols, SFI)
	ADD_REG("+", SFI_POSITIVE, 1, "����", "���磺+x")
	ADD_REG("-", SFI_NEGATIVE, 1, "����", "���磺-x")
	ADD_REG("!", SFI_NOT, 1, "��", "���磺!x")
	ADD_REG("~", SFI_BIT_NOT, 1, "λ��", "���磺~x �ò�����xת��Ϊlong����")
DEFINE_SYMBOLS_END

//�Ӽ�������
DEFINE_SYMBOLS_START(AdditiveOpSymbols, SFI)
	ADD_REG("+", SFI_PLUS, 2, "��", "���磺x+y")
	ADD_REG("-", SFI_MINUS, 2, "��", "���磺x-y")
DEFINE_SYMBOLS_END

//�˳�ȡ�������
DEFINE_SYMBOLS_START(MultiplicativeOpSymbols, SFI)
	ADD_REG("*", SFI_MUL, 2, "��", "���磺x*y")
	ADD_REG("/", SFI_DIV, 2, "��", "���磺x/y")
	ADD_REG("%", SFI_MOD, 2, "ȡģ", "���磺x%y �ò�����x,yת��Ϊlong����")
DEFINE_SYMBOLS_END


DEFINE_SYMBOLS_START(RelationalOpSymbols, SFI)
	ADD_REG("<", SFI_LESS, 2, "С��", "���磺x<y")
	ADD_REG("<=", SFI_LESS_EQU, 2, "С�ڵ���", "���磺x<=y")
	ADD_REG(">", SFI_GREATER, 2, "����", "���磺x>y")
	ADD_REG(">=", SFI_GREATER_EQU,2, "���ڵ���", "���磺x>=y")
DEFINE_SYMBOLS_END

DEFINE_SYMBOLS_START(BitAndOpSymbols, SFI)
	ADD_REG("&", SFI_BIT_AND, 2, "λ��", "���磺x&y, �ò�����x,yת��Ϊlong����")
DEFINE_SYMBOLS_END

DEFINE_SYMBOLS_START(BitXOROpSymbols, SFI)
	ADD_REG("^", SFI_BIT_XOR, 2, "λ���", "���磺x^y, �ò�����x,yת��Ϊlong����")
DEFINE_SYMBOLS_END

DEFINE_SYMBOLS_START(BitOrOpSymbols, SFI)
	ADD_REG("|", SFI_BIT_OR, 2, "λ��", "���磺x|y, �ò�����x,yת��Ϊlong����")
DEFINE_SYMBOLS_END

DEFINE_SYMBOLS_START(EqulityOpSymbols, SFI)
	ADD_REG("=", SFI_EQU, 2, "����", "���磺x=y, ���� 0��1")
	ADD_REG("<>", SFI_NOT_EQU, 2, "������", "���磺x<>y, ���� 0��1")
	ADD_REG("!=", SFI_NOT_EQU_N, 2, "������", "���磺x<>y, ���� 0��1")
DEFINE_SYMBOLS_END

DEFINE_SYMBOLS_START(LogicalOrOpSymbols, SFI)
	ADD_REG("||", SFI_OR_V, 2, "�߼���", "���磺x || y, ���� 0��1")
	ADD_REG("or", SFI_OR, 2, "�߼���", "���磺x or y, ���� 0��1")
DEFINE_SYMBOLS_END


DEFINE_SYMBOLS_START(LogicalAndOpSymbols, SFI)
	ADD_REG("&&", SFI_AND_A, 2, "�߼���", "���磺x && y, ���� 0��1")
	ADD_REG("and", SFI_AND, 2, "�߼���", "���磺x and y, ���� 0��1")
DEFINE_SYMBOLS_END