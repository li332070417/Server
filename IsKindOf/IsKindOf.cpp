// IsKindOf.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <assert.h>

#define _RUNTIME_CLASS(class_name) ((CRunTimeClass*)(&class_name::class##class_name))
#define _INIT_RUNTIMECLASS2(class_name, base_class_name) \
	const struct CRunTimeClass class_name::class##class_name =\
	{ #class_name, sizeof(class_name), 0xffff, base_class_name::GetThisClass() };
#define _INIT_RUNTIMECLASS1(class_name) \
	const struct CRunTimeClass class_name::class##class_name =\
	{ #class_name, sizeof(class_name), 0xffff, NULL };
#define DECLARE_DYNAMIC(class_name)\
	static const struct CRunTimeClass class##class_name;\
	virtual CRunTimeClass* GetRuntimeClass()\
	{\
		return _RUNTIME_CLASS(class_name);\
	}\
	static CRunTimeClass* GetThisClass()\
	{\
		return _RUNTIME_CLASS(class_name);\
	}

using namespace std;

struct CRunTimeClass
{
	char* m_lpszClassName;
	int m_nObjectSize;
	unsigned int m_wSchema;
	CRunTimeClass* m_pBaseClass;
};

class CGrandFather
{
public:
	virtual bool IsKindOf(const CRunTimeClass* pClass)
	{
		assert(pClass != NULL);
		CRunTimeClass* pClassThis = GetRuntimeClass();
		while (pClassThis != NULL)
		{
			if (pClassThis == pClass)
				return true;
			char* a = pClassThis->m_lpszClassName;
			pClassThis = pClassThis->m_pBaseClass;	
		}
		return false;
	}
	DECLARE_DYNAMIC(CGrandFather)
};

class CFather:public CGrandFather
{
public:
	DECLARE_DYNAMIC(CFather)
};

class CUncle:public CGrandFather
{
public:
	DECLARE_DYNAMIC(CUncle)
};

class CSon:public CFather
{
public:
	DECLARE_DYNAMIC(CSon)
};

class CBrother:public CUncle
{
public:
	DECLARE_DYNAMIC(CBrother)
};

_INIT_RUNTIMECLASS1(CGrandFather)
_INIT_RUNTIMECLASS2(CFather, CGrandFather)
_INIT_RUNTIMECLASS2(CUncle, CGrandFather)
_INIT_RUNTIMECLASS2(CSon, CFather)
_INIT_RUNTIMECLASS2(CBrother, CUncle)

int _tmain(int argc, _TCHAR* argv[])
{
	CSon son;
	CFather father;
	bool bKind = son.IsKindOf(CUncle::GetThisClass());
	cout << "son IsKindOf Uncle:" << bKind << endl;
	bKind = son.IsKindOf(CGrandFather::GetThisClass());
	cout << "son IsKindOf GrandFather:" << bKind << endl;
	bKind = son.IsKindOf(CBrother::GetThisClass());
	cout << "son IsKindOf Brother:" << bKind << endl;
	bKind = father.IsKindOf(CGrandFather::GetThisClass());
	cout << "father IsKindOf GrandFather:" << bKind << endl;
	bKind = father.IsKindOf(CSon::GetThisClass());
	cout << "father IsKindOf son:" << bKind << endl;
	system("pause");
	return 0;
}