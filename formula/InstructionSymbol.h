#pragma once
#include <cstdint>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
using namespace std;

#pragma pack(push, 1)



struct Var {
	//��������
	enum VarType {
		VAR_TYPE_SINGLE,
		VAR_TYPE_SERIES
	};

	//��������
	enum DataType {
		DATA_TYPE_NUMBER = 1,	//��������
		DATA_TYPE_STRING		//�ַ���
	};

	Var() {
		setVarType(VAR_TYPE_SINGLE);
		setDataType(DATA_TYPE_NUMBER);
		memset(name, 0, sizeof(name));
	}
	virtual ~Var() {}
	/*
	 *	���ر�������
	 */
	VarType getVarType() { return varType; }
	/*
	 *	�ж��Ƿ����������ͱ���
	 *	@ret true ���������ͱ����� false �ǵ�ֵ���ͱ���
	 */
	bool isSeriesVar() { return varType == VAR_TYPE_SERIES; }
	/*
	 *	���ر�������������
	 *
	 */
	DataType getDataType() { return dataType; }
	/*
	 *	�ж��Ƿ�����������
	 *	@ret true ���������ͣ� false ���ַ�������
	 */
	bool isNumberData() { return dataType == DATA_TYPE_NUMBER; }
	/*
	 *	���ñ���������
	 */
	void setName(const char* n) {
		if (n) {
			memcpy(this->name, n, std::min(strlen(n), sizeof(this->name)));
		}
	}
	/*
	 *	���ر���������
	 */
	const char* getName() { return name; }

public:
	//�������ݳ���
	virtual size_t getDataLen() {
		return 0;
	}
	//��������
	virtual double getValue(size_t pos = 0) {
		return 0.f;
	}
	//�����ַ���
	virtual const char* getStringValue(size_t pos = 0) {
		return nullptr;
	}
	//������������
	virtual void setValue(double v) {}
	//����ָ��λ�õ���������
	virtual void setValue(double v, size_t pos) {}
	//����������������
	virtual void setValue(double* v, size_t len) {}
	//�����ַ���
	virtual void setValue(const char* v) {}



protected:
	//���ñ�������
	void setVarType(VarType type) { varType = type; }
	//������������
	void setDataType(DataType type) { dataType = type; }

protected:
	char		name[32];		//����
	VarType		varType;		//��������
	DataType	dataType;		//��������
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

