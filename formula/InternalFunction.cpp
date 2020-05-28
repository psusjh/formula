#include "stdafx.h"
#include <algorithm>
#include <assert.h>
#include <string>
using namespace std;
#include "InternalFunction.h"
#include "FormulaDef.h"

#define REGISTER_INTERNAL_FUN(id, name, num, cn, usage) \
	InternalFunctionSingleton::instance().registerFunction(id, name, 0, num, cn, usage);

void InternalFunction::initCatalog() {
	int catalogSize = sizeof(sfiCatalog) / sizeof(SfiCatalog);
	for (int i =  0; i < catalogSize; ++i) {
		if (i <= catalogSize - 2) {
			InternalFunctionSingleton::instance().registerCatalog(sfiCatalog[i].name, sfiCatalog[i].sfi, sfiCatalog[i+1].sfi);
		}
		else {
			InternalFunctionSingleton::instance().registerCatalog(sfiCatalog[i].name, sfiCatalog[i].sfi, 0xff);
		}
	}
}


void InternalFunction::initInternalFunction()
{
	REGISTER_INTERNAL_FUN(SFI_JUMP, "jump", 1, "��ת", "");
	REGISTER_INTERNAL_FUN(SFI_LOOP, "loop", 1, "ѭ��", "");
	REGISTER_INTERNAL_FUN(SFI_JEQ, "jumpEQ", 2, "�����ת", "");
	REGISTER_INTERNAL_FUN(SFI_JNEQ, "jumpNEQ", 2, "�������ת", "");
	REGISTER_INTERNAL_FUN(SFI_JGT, "jumpGT", 3, "������ת", "");
	REGISTER_INTERNAL_FUN(SFI_JGE, "jumpGE", 3, "���ڵ�����ת", "");
	REGISTER_INTERNAL_FUN(SFI_JLT, "jumpLT", 3, "С����ת", "");
	REGISTER_INTERNAL_FUN(SFI_JLE, "jumpLE", 3, "С�ڵ�����ת", "");

	REGISTER_INTERNAL_FUN(SFI_ASSIGN, ":=", 1, "��ֵ", "���磺x:=1");
	REGISTER_INTERNAL_FUN(SFI_OUT, ":", 1, "���", "���磺x:1");

	REGISTER_INTERNAL_FUN(SFI_O, "O", 0, "���̼�", "˵����ȡ�ÿ��̼�\n�÷���\nO");
	REGISTER_INTERNAL_FUN(SFI_OPEN, "OPEN", 0, "���̼�", "˵����ȡ�ÿ��̼�\n�÷���\nOPEN");
	REGISTER_INTERNAL_FUN(SFI_H, "H", 0, "��߼�", "˵����ȡ����߼�\n�÷���\nH");
	REGISTER_INTERNAL_FUN(SFI_HIGH, "HIGH", 0, "��߼�", "˵����ȡ����߼�\n�÷���\nHIGH");
	REGISTER_INTERNAL_FUN(SFI_L, "L", 0, "��ͼ�", "˵����ȡ����߼�\n�÷���\nL");
	REGISTER_INTERNAL_FUN(SFI_LOW, "LOW", 0, "��ͼ�", "˵����ȡ����߼�\n�÷���\nLOW");
	REGISTER_INTERNAL_FUN(SFI_C, "C", 0, "���̼�", "˵����ȡ����߼�\n�÷���\nC");
	REGISTER_INTERNAL_FUN(SFI_CLOSE, "CLOSE", 0, "���̼�", "˵����ȡ����߼�\n�÷���\nCLOSE");

	REGISTER_INTERNAL_FUN(SFI_MA, "MA", 2, "���ƶ�ƽ����", "˵����������ƶ�ƽ����\n�÷���\n MA(X,N):X��N�ռ��ƶ�ƽ��,�㷨(X1+X2+X3+...+Xn)/N,N֧�ֱ���");
	REGISTER_INTERNAL_FUN(SFI_IF, "IF", 3, "ѡ����", "˵���������������ز�ͬ��ֵ\n�÷���\n IF(X,A,B)������X��Ϊ0�򷵻�A,���򷵻�B \n���磺IF(CLOSE > OPEN, HIGH, LOW)��ʾ�����������򷵻����ֵ�����򷵻����ֵ");

}

InternalFunction::InternalFunction()
{
}


InternalFunction::~InternalFunction()
{
}

void InternalFunction::registerFunction(uint16_t id, const char* name, uint16_t attr, uint8_t paramNum, const char* cnName, const char* usage)
{
	assert(catalogList.size());

	Function* function = new Function();
	memset(function, 0, sizeof(Function));
	function->id = id;
	function->instructAttr = attr;
	function->paramNum = paramNum;
	if (name) {
		memcpy(function->name, name, std::min(strlen(name), sizeof(function->name) - 1));
	}
	if (cnName) {
		memcpy(function->cnName, cnName, std::min(strlen(cnName), sizeof(function->cnName) - 1));
	}
	if (usage) {
		memcpy(function->usage, usage, std::min(strlen(usage), sizeof(function->usage) - 1));
	}
	functionMap.insert(make_pair(id, shared_ptr<Function>(function)));
	funcName2id.insert(make_pair(function->name, id));
	addCatalogMember(id);
	
}

void InternalFunction::registerCatalog(const char* name, uint16_t start, uint16_t end)
{
	bool find = false;
	for (auto e : catalogList) {
		if (start < e->end ) {
			find = true;
			break;
		}
	}
	if (!find) {
		Catalog* catalog = new Catalog();
		catalog->catalog = name;
		catalog->start = start;
		catalog->end = end;
		catalogList.push_back(shared_ptr<Catalog>(catalog));
	}
}

uint16_t InternalFunction::getStartCatalog(uint16_t id)
{
	for (auto e : catalogList) {
		if (e->start <= id && e->end > id) {
			return e->start;
		}
	}
	assert(0);
	return SFI_VOID;
}

FunctionPtr InternalFunction::getFunction(uint16_t id)
{
	unordered_map< uint16_t, shared_ptr<Function>>::iterator itf = functionMap.find(id);
	if (itf != functionMap.end()) {
		return itf->second;
	}
	assert(0);
	return nullptr;
}

FunctionPtr InternalFunction::getFunction(const char* name)
{
	unordered_charptr_map::iterator itf = funcName2id.find(name);
	if (itf != funcName2id.end()) {
		return getFunction(itf->second);
	}
	assert(0);
	return nullptr;
}

std::string InternalFunction::getFunctionName(uint16_t id)
{
	shared_ptr<Function> fun = getFunction(id);
	if (fun) {
		return fun->name;
	}
	return std::to_string(id);
}

void InternalFunction::addCatalogMember(uint16_t id){
	for (auto e : catalogList) {
		if (id>= e->start && id< e->end) {
			e->elements.push_back(id);
			break;
		}
	}
}
