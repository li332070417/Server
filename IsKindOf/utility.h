#pragma once
#include <stdio.h>

namespace Utility
{
#define SAFE_DELETE(p) if (p) delete p;
	class noncopyable
	{
	protected:
		noncopyable(){}
		virtual ~noncopyable(){}
	private:
		noncopyable& operator=(const noncopyable&);
		noncopyable(const noncopyable&);
	};

#define SINGLETON_DEF(class_name) \
protected:\
	class_name(){} \
public:\
	static class_name* GetInstance()\
	{\
		static class_name* p = NULL; \
		return NULL == p ? p = new class_name : p; \
	}

	class mempoolobj
	{
	public:
		static void* operator new(size_t iSize)
		{
			return malloc(iSize);
		}
		static void operator delete(void *p)
		{
			SAFE_DELETE(p)
		}
		static void* operator new(size_t iSize, int iCount)
		{
			return malloc(iSize * iCount);
		}
	};
}