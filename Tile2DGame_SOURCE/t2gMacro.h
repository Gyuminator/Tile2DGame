#pragma once

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }

#define GET_SINGLETON(name) t2g::SingletonBase<t2g::name>::GetInst()

#define GET_COMPONENT(name) GetComponent<t2g::name>(t2g::enums::eComponentType::name)

