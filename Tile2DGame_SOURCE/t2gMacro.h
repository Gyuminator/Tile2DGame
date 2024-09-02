#pragma once

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }

#define GET_SINGLETON(name) t2g::SingletonBase<t2g::name>::GetInst()

#define SINGLETON(name) \
public:\
	static name& GetInst()\
	{\
		if (inst == nullptr)\
			inst = new name();\
		return *inst;\
	}\
	static void DeleteInst()\
	{\
		SAFE_DELETE(inst);\
	}\
private:\
	name() {};\
	~name() {};\
	name(name&) = delete;\
	name& operator=(const name&) = delete;\
	\
	static name* inst;