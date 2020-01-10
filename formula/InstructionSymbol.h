#pragma once
#include <cstdint>
#include <algorithm>

#define LITERAL_TYPE_CONST	0x1
#define LITERAL_TYPE_STR	0x2
#define LITERAL_TYPE_VAR	0x3

struct LiteralVar 
{
	char		name[32];
	uint32_t	len;
	double*		values;
};

struct Literal {
	Literal(double v) {
		literalType = LITERAL_TYPE_CONST;
		literalValue.value = v;
	}
	Literal(const char* str) {
		literalType = LITERAL_TYPE_STR;
		int len = strlen(str);

		literalValue.str = new char[len + 1];
		memset(literalValue.str, 0, len + 1);
		memcpy(literalValue.str, 0, len);
	}

	Literal(const char* name, uint32_t len, double* values) {
		memset(&literalValue.var, 0, sizeof(LiteralVar));
		literalType = LITERAL_TYPE_VAR;
		memcpy(literalValue.var.name, name, std::min((int)strlen(name), 32));
		literalValue.var.len = len;
		if (len) {
			literalValue.var.values = new double[len];
			memcpy(literalValue.var.values, values, len * sizeof(double));
		}		
	}

	
	~Literal() {
		if (literalType == LITERAL_TYPE_STR) {
			if (literalValue.str) {
				delete[]literalValue.str;
			}
		}
		else if (literalType == LITERAL_TYPE_VAR) {
			if (literalValue.var.values) {
				delete[]literalValue.var.values;
			}
		}
	}

	bool operator ==(const Literal& rhs) {
		if (this == &rhs) {
			return true;
		}
		if (this->literalType != rhs.literalType) {
			return false;
		}
		if (this->literalType == LITERAL_TYPE_STR) {
			return strcmp(this->literalValue.str, rhs.literalValue.str) == 0;
		}
		else if (this->literalType == LITERAL_TYPE_CONST) {
			return this->literalValue.value == rhs.literalValue.value;
		}
		return false;
	}


	
	uint8_t literalType;
	union 
	{
		LiteralVar var;
		double value;
		char*  str;
	} literalValue; 
};

struct OperatorSymbol {
	uint16_t id;

};
struct InstructionSymbol {
	uint16_t id;
	uint8_t	numParam;
	uint32_t* literalPos;
};


