#pragma once
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