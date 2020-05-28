#pragma once
#include <cstdint>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
using namespace std;

#pragma pack(push, 1)



struct Var {
	//变量类型
	enum VarType {
		VAR_TYPE_SINGLE,
		VAR_TYPE_SERIES
	};

	//数据类型
	enum DataType {
		DATA_TYPE_NUMBER = 1,	//数字类型
		DATA_TYPE_STRING		//字符串
	};

	Var() {
		setVarType(VAR_TYPE_SINGLE);
		setDataType(DATA_TYPE_NUMBER);
		memset(name, 0, sizeof(name));
	}
	virtual ~Var() {}
	/*
	 *	返回变量类型
	 */
	VarType getVarType() { return varType; }
	/*
	 *	判断是否是序列类型变量
	 *	@ret true 是序列类型变量， false 是单值类型变量
	 */
	bool isSeriesVar() { return varType == VAR_TYPE_SERIES; }
	/*
	 *	返回变量的数据类型
	 *
	 */
	DataType getDataType() { return dataType; }
	/*
	 *	判断是否是数字类型
	 *	@ret true 是数字类型， false 是字符串类型
	 */
	bool isNumberData() { return dataType == DATA_TYPE_NUMBER; }
	/*
	 *	设置变量的名称
	 */
	void setName(const char* n) {
		if (n) {
			memcpy(this->name, n, std::min(strlen(n), sizeof(this->name)));
		}
	}
	/*
	 *	返回变量的名称
	 */
	const char* getName() { return name; }

public:
	//返回数据长度
	virtual size_t getDataLen() {
		return 0;
	}
	//返回数据
	virtual double getValue(size_t pos = 0) {
		return 0.f;
	}
	//返回字符串
	virtual const char* getStringValue(size_t pos = 0) {
		return nullptr;
	}
	//设置数字数据
	virtual void setValue(double v) {}
	//设置指定位置的数字数据
	virtual void setValue(double v, size_t pos) {}
	//设置数字序列数据
	virtual void setValue(double* v, size_t len) {}
	//设置字符串
	virtual void setValue(const char* v) {}



protected:
	//设置变量类型
	void setVarType(VarType type) { varType = type; }
	//设置数据类型
	void setDataType(DataType type) { dataType = type; }

protected:
	char		name[32];		//名称
	VarType		varType;		//变量类型
	DataType	dataType;		//数据类型
};

typedef shared_ptr<Var> VarPtr;

struct SingleVar : Var {
	SingleVar() {
		setVarType(Var::VAR_TYPE_SINGLE);
	}
public:
	virtual size_t getDataLen() { return 1; }
};

struct SingleNumberVar : SingleVar {
	SingleNumberVar() {
		value = 0.0;
	}
	SingleNumberVar(const char* name) {
		setName(name);
		value = 0.0;
	}

	SingleNumberVar(double v) {
		value = v;
	}

	SingleNumberVar(const char* name, double v) {
		setName(name);
		value = v;
	}
public:
	virtual double getValue(size_t pos = 0) { return value; }

	virtual void setValue(double v) { value = v; }
protected:
	double value;
};

struct SingleStringVar :SingleVar {
	SingleStringVar() {
		setDataType(Var::DATA_TYPE_STRING);
	}

	SingleStringVar(const char* v, const char* name = nullptr) {
		setDataType(Var::DATA_TYPE_STRING);
		setName(name);
		if (v) {
			value = v;
		}
	}
public:
	virtual const char* getStringValue(size_t pos = 0) { return value.c_str(); }

	virtual void setValue(const char* v) {
		if (v) {
			value = v;
		}
	}
protected:
	string value;
};

struct SeriesVar :Var {
	SeriesVar() {
		setVarType(Var::VAR_TYPE_SERIES);
		dataLen = 0;
	}
public:
	virtual size_t getDataLen() { return dataLen; }
protected:
	size_t dataLen;
};

struct SeriesNumberVar : SeriesVar {
	SeriesNumberVar() {
		value = nullptr;
	}
	SeriesNumberVar(double* v, size_t len) {
		setValue(v, len);
	}

	SeriesNumberVar(const char* name) {
		setName(name);
	}

	SeriesNumberVar(const char* name, double* v, size_t len) {
		setName(name);
		setValue(v, len);
	}
	virtual ~SeriesNumberVar() {
		if (dataLen) {
			delete[]value;
		}
	}
public:
	virtual void setValue(double* v, size_t len) {
		if (dataLen) {
			delete[]value;
		}
		dataLen = len;
		value = new double[len];
		memmove(value, v, len * sizeof(double));
	}
	virtual double getValue(size_t pos = 0) {
		if (pos < dataLen)
			return value[pos];
		return 0.f;
	}
protected:
	double* value;
};

struct InstructionSymbol{
	uint16_t id;
	uint8_t	numParam;
	uint32_t* literalPos;
	uint32_t resultPos;
	virtual ~InstructionSymbol() {
		if (literalPos) {
			delete[]literalPos;
		}
	}
};

typedef shared_ptr<InstructionSymbol> InstructionSymbolPtr;

struct BlockSymbol {	
	size_t id;
	BlockSymbol* parent;
	vector<InstructionSymbolPtr> symbols;
	vector<BlockSymbol*> groups;
};


#pragma pack(pop)

