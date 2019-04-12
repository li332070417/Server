#include "stdafx.h"
#include <windows.h>
#include "utility.h"
#include "iskind.h"
#include <atomic>
#include <vector>
#include <memory>
#include <map>

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

class CFather :public CGrandFather
{
public:
	DECLARE_DYNAMIC(CFather)
};

class CUncle :public CGrandFather
{
public:
	DECLARE_DYNAMIC(CUncle)
};

class CSon :public CFather
{
public:
	DECLARE_DYNAMIC(CSon)
};

class CBrother :public CUncle
{
public:
	DECLARE_DYNAMIC(CBrother)
};

_INIT_RUNTIMECLASS1(CGrandFather)
_INIT_RUNTIMECLASS2(CFather, CGrandFather)
_INIT_RUNTIMECLASS2(CUncle, CGrandFather)
_INIT_RUNTIMECLASS2(CSon, CFather)
_INIT_RUNTIMECLASS2(CBrother, CUncle)

class CManager
{
	SINGLETON_DEF(CManager)
private:
	int a;
};

class CMemTest : public Utility::mempoolobj
{
public:
	CMemTest(){ cout << "Construct" << endl; }
	~CMemTest(){ cout << "Free" << endl; }
	void operator delete[](void* p)
	{
		return;
	}
// 	void* operator new(size_t iSize)
// 	{
// 		cout << "class construct" << endl;
// 		return malloc(iSize);
// 	}
// 	void* operator new(size_t iSize, void* p)
// 	{
// 		return p;
// 	}
};

// void* operator new(size_t iSize)
// {
// 	cout << "Global new" << endl;
// 	return malloc(iSize);
// }

class Noncopy : public Utility::noncopyable
{

};

void operator delete[](void* p, size_t iSize)
{
	return;
}

class CA
{
public:
	int a;
};

class CMemDriver : public CMemTest
{
public:
	CMemDriver(){ cout << "CMemDriver Construct" << endl; }
	~CMemDriver(){ cout << "CMemDriver Free" << endl; }
};

int _tmain(int argc, _TCHAR* argv[])
{
	//runtime test
// 	CSon son;
// 	CFather father;
// 	bool bKind = son.IsKindOf(CUncle::GetThisClass());
// 	cout << "son IsKindOf Uncle:" << bKind << endl;
// 	bKind = son.IsKindOf(CGrandFather::GetThisClass());
// 	cout << "son IsKindOf GrandFather:" << bKind << endl;
// 	bKind = son.IsKindOf(CBrother::GetThisClass());
// 	cout << "son IsKindOf Brother:" << bKind << endl;
// 	bKind = father.IsKindOf(CGrandFather::GetThisClass());
// 	cout << "father IsKindOf GrandFather:" << bKind << endl;
// 	bKind = father.IsKindOf(CSon::GetThisClass());
// 	cout << "father IsKindOf son:" << bKind << endl;
	//utility test
	//CManager* pMgr = CManager::GetInstance();
	std::map<int,int> map;
	map[1] = 10;
	map[2] = 20;
// 	std::shared_ptr<CA> ptr = make_shared<CA>();
// 	ptr->a = 10;
// 	std::weak_ptr<CA> weakptr = ptr;
// 	std::shared_ptr<CA> ptr2 = weakptr.lock();
// 	std::vector<CMemTest> vMem;
// 	CMemTest m1;
// 	CMemTest* pM1 = &m1;
// 	vMem.push_back(std::move(m1));
// 	pM1 = &vMem[0];
	Noncopy no;
	int iSize = sizeof(CMemDriver);
	CMemDriver* pMem = new CMemDriver[5];
	delete [] pMem;
// 	CMemTest* pMemAlloc = (CMemTest*)operator new(sizeof(CMemTest));
// 	//void * pMemAlloc = malloc(sizeof(CMemTest));
//  	new(pMemAlloc) CMemTest();
//  	pMemAlloc->~CMemTest();
//  	operator delete(pMemAlloc);
	system("pause");
	return 0;
}