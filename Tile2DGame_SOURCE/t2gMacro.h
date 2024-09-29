#pragma once

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }

#define GET_SINGLETON(name) t2g::SingletonBase<t2g::name>::GetInst()

#define GET_COMPONENT(name) GetComponent<t2g::name>(t2g::enums::eComponentType::name)


//#define NAMED_ENUM() InitIndex();
//#define NAME(name) = RegisterName(name)
//#define REGISTER_NAMED_ENUM(eType) RegisterNamedEnum(eType);
//
//class SaveEnumStrMap
//{
//	friend consteval void InitIndex();
//	friend consteval UINT RegisterName(const wchar_t* name);
//	friend consteval void RegisterNamedEnum();
//
//private:
//	static UINT AccIndex;
//	static const wchar_t* WstrArray[100];
//};
//
//consteval void InitIndex() { SaveEnumStrMap::AccIndex = 0; }
//
//consteval UINT RegisterName(const wchar_t* name)
//{
//	SaveEnumStrMap::WstrArray[SaveEnumStrMap::AccIndex] = name;
//	return SaveEnumStrMap::AccIndex++;
//}
//
//template<typename eType>
//class EnumWchar
//{
//public:
//	static const wchar_t* values[(INT)eType::EnumEnd];
//};
//
//template<typename eType>
//consteval void RegisterNamedEnum()
//{
//	for (int i = 0; i < (int)eType::EnumEnd; ++i)
//	{
//		EnumWchar<eType>::values[i] = SaveEnumStrMap::WstrArray[i];
//	}
//}

