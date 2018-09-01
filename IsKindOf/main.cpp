#include "stdafx.h"
#include <windows.h>
#include "utility.h"
#include "iskind.h"

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
};

int _tmain(int argc, _TCHAR* argv[])
{
	//runtime test
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
	//utility test
	CManager* pMgr = CManager::GetInstance();
	CMemTest* pMem = new CMemTest[10];
	system("pause");
	return 0;
}