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
		public:
		noncopyable& operator=(const noncopyable&) = delete;
		noncopyable(const noncopyable&) = delete;
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
		void* operator new(size_t iSize)
		{
			return malloc(iSize);
		}
// 		static void* operator new(size_t iSize, int iCount)
// 		{
// 			return malloc(iSize * iCount);
// 		}
		static void* operator new(size_t iSize, void* p)
		{
			return p;
		}
		static void* operator new(size_t iSize, int i)
		{
			return malloc(iSize);
		}
		void* operator new[](size_t iSize)
		{
			return malloc(iSize);
		}

		void operator delete(void *p)
		{
			SAFE_DELETE(p)
		}
		static void operator delete(void* p, void*)
		{
			SAFE_DELETE(p);
		}
		static void operator delete[](void* p, size_t iSize)
		{
			free(p);
			return;
		}
		static void operator delete[](void *p)
		{
			free(p);
			return;
		}
	};
}