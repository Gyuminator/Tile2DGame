#pragma once

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }

#define GET_SINGLETON(name) t2g::SingletonBase<t2g::name>::GetInst()

