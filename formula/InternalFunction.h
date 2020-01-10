#pragma once
#include <cstdint>
#include <unordered_map>
#include <list>

#include <map>
#include <memory>
using namespace std;



#pragma pack(push,1)

struct Function
{
	uint16_t		id;
	char			name[16];
	uint16_t		instructAttr;
	uint8_t			paramNum;
	char			cnName[32];
	char			usage[128];
};
#pragma pack(pop)


class InternalFunction
{
	struct Catalog{		
		uint16_t			start;
		uint16_t			end;
		string				catalog;
		vector<uint16_t>	elements;
	};
public:
	InternalFunction();
	~InternalFunction();
public:
	void registerFunction(uint16_t id, char* name, uint16_t attr, uint8_t paramNum, char* cnName, char* usage);
	void registerCatalog(char* name, uint16_t start, uint16_t end);
protected:
	void addCatalogMember(uint16_t id);
protected:
	unordered_map<uint16_t, shared_ptr<Function>> functionMap;
	list<shared_ptr<Catalog>> catalogList;

	
};

