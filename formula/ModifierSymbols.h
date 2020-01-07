#pragma once
#include "SymbolsDef.h"
#include "FormulaDef.h"
//颜色修饰
DEFINE_SYMBOLS_START(ColorSymbols, uint32_t)
	ADD("red", RGB2INT(255, 0, 0))
	ADD("green", RGB2INT(0, 255, 0))
	ADD("blue", RGB2INT(0, 0, 255))
	ADD("black", RGB2INT(0, 0, 0))
	ADD("white", RGB2INT(255, 255, 255))
	ADD("gray", RGB2INT(128, 128, 128))
	ADD("yellow", RGB2INT(255, 255, 0))
	ADD("cyan", RGB2INT(0, 255, 255))
	ADD("magenta", RGB2INT(255, 0, 255))
	ADD("brown", RGB2INT(128, 0, 0))
DEFINE_SYMBOLS_END

//线条类型
DEFINE_SYMBOLS_START(LineTypeSymbols, uint8_t)
	ADD("stick", 0)
	ADD("colorstick", 1)
	ADD("volstick", 2)
	ADD("linestick", 3)
	ADD("crossdot", 4)
	ADD("circledot", 5)
	ADD("pointdot", 6)
	ADD("stick3d", 7)
	ADD("color3d", 8)
	ADD("dotline", 9)
	ADD("dashline", 10)
DEFINE_SYMBOLS_END
