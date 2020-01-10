#include "stdafx.h"
#include <algorithm>
#include <assert.h>
#include "InternalFunction.h"


InternalFunction::InternalFunction()
{
}


InternalFunction::~InternalFunction()
{
}

void InternalFunction::registerFunction(uint16_t id, char* name, uint16_t attr, uint8_t paramNum, char* cnName, char* usage)
{
	assert(catalogList.size());

	Function* function = new Function();
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
	addCatalogMember(id);
	
}

void InternalFunction::registerCatalog(char* name, uint16_t start, uint16_t end)
{
	bool find = false;
	for (auto e : catalogList) {
		if (e->start <= start || e->end >= end) {
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

void InternalFunction::addCatalogMember(uint16_t id){
	for (auto e : catalogList) {
		if (id>= e->start && id<= e->end) {
			e->elements.push_back(id);
			break;
		}
	}
}
