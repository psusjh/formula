#pragma once
#include <cstdint>
#include <unordered_map>
#include <list>

#include <map>
#include <memory>
#include <string>
using namespace std;

#include <boost/thread/detail/singleton.hpp>



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
typedef shared_ptr<Function> FunctionPtr;
#pragma pack(pop)



class InternalFunction
{
	struct Catalog {
		uint16_t			start;
		uint16_t			end;
		string				catalog;
		vector<uint16_t>	elements;
	};
protected:
	InternalFunction();
	~InternalFunction();
public:
	void registerFunction(uint16_t id, const char* name, uint16_t attr, uint8_t paramNum, const char* cnName, const char* usage);
	void registerCatalog(const char* name, uint16_t start, uint16_t end);
	uint16_t getStartCatalog(uint16_t id);
	/*
	 *	根据名字获得Function
	 *	@name 
	 *	@ret
	 */
	FunctionPtr getFunction(const char* name);
	/*
	 *	根据id获得Function
	 *	@id 
	 *	@ret
	 */
	FunctionPtr getFunction(uint16_t id);
	/*
	 *	根据id返回对应的名字
	 *	@id 指令id
	 *	@ret 返回对应的名字
	 */
	string getFunctionName(uint16_t id);
protected:
	void addCatalogMember(uint16_t id);
protected:
	unordered_map<uint16_t, shared_ptr<Function>> functionMap;
	
	struct StringNoCaseCompare{
		bool operator()(const char* a, const char* b)const {
#ifdef _WIN32
			return _stricmp(a, b) == 0;
#else
			return strcasecmp(a, b) == 0;
#endif
		}
	};
	struct HashCharPtr {
		size_t operator()(const char* str) const {
			string temp(str);
			std::transform(temp.begin(), temp.end(), temp.begin(), tolower);
 			return std::_Hash_array_representation((const unsigned char*)temp.c_str(), temp.size());
		}
	};
	typedef unordered_map<const char*, uint16_t, HashCharPtr, StringNoCaseCompare > unordered_charptr_map;
	unordered_charptr_map funcName2id;
	list<shared_ptr<Catalog>> catalogList;

public:
	static void initCatalog();
	static void initInternalFunction();

};

typedef public boost::detail::thread::singleton<InternalFunction> InternalFunctionSingleton;

