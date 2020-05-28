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
	REGISTER_INTERNAL_FUN(SFI_JUMP, "jump", 1, "跳转", "");
	REGISTER_INTERNAL_FUN(SFI_LOOP, "loop", 1, "循环", "");
	REGISTER_INTERNAL_FUN(SFI_JEQ, "jumpEQ", 2, "相等跳转", "");
	REGISTER_INTERNAL_FUN(SFI_JNEQ, "jumpNEQ", 2, "不相等跳转", "");
	REGISTER_INTERNAL_FUN(SFI_JGT, "jumpGT", 3, "大于跳转", "");
	REGISTER_INTERNAL_FUN(SFI_JGE, "jumpGE", 3, "大于等于跳转", "");
	REGISTER_INTERNAL_FUN(SFI_JLT, "jumpLT", 3, "小于跳转", "");
	REGISTER_INTERNAL_FUN(SFI_JLE, "jumpLE", 3, "小于等于跳转", "");

	REGISTER_INTERNAL_FUN(SFI_ASSIGN, ":=", 1, "赋值", "例如：x:=1");
	REGISTER_INTERNAL_FUN(SFI_OUT, ":", 1, "输出", "例如：x:1");

	REGISTER_INTERNAL_FUN(SFI_O, "O", 0, "开盘价", "说明：取得开盘价\n用法：\nO");
	REGISTER_INTERNAL_FUN(SFI_OPEN, "OPEN", 0, "开盘价", "说明：取得开盘价\n用法：\nOPEN");
	REGISTER_INTERNAL_FUN(SFI_H, "H", 0, "最高价", "说明：取得最高价\n用法：\nH");
	REGISTER_INTERNAL_FUN(SFI_HIGH, "HIGH", 0, "最高价", "说明：取得最高价\n用法：\nHIGH");
	REGISTER_INTERNAL_FUN(SFI_L, "L", 0, "最低价", "说明：取得最高价\n用法：\nL");
	REGISTER_INTERNAL_FUN(SFI_LOW, "LOW", 0, "最低价", "说明：取得最高价\n用法：\nLOW");
	REGISTER_INTERNAL_FUN(SFI_C, "C", 0, "收盘价", "说明：取得最高价\n用法：\nC");
	REGISTER_INTERNAL_FUN(SFI_CLOSE, "CLOSE", 0, "收盘价", "说明：取得最高价\n用法：\nCLOSE");

	REGISTER_INTERNAL_FUN(SFI_MA, "MA", 2, "简单移动平均线", "说明：计算简单移动平均线\n用法：\n MA(X,N):X的N日简单移动平均,算法(X1+X2+X3+...+Xn)/N,N支持变量");
	REGISTER_INTERNAL_FUN(SFI_IF, "IF", 3, "选择函数", "说明：根据条件返回不同的值\n用法：\n IF(X,A,B)若条件X不为0则返回A,否则返回B \n例如：IF(CLOSE > OPEN, HIGH, LOW)表示该周期收阳则返回最高值，否则返回最低值");

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
