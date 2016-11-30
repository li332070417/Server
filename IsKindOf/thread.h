#include "stdafx.h"
#include "noncopyable.h"
#include <afxmt.h>

class CThread : private CNoncopyable
{
public:
	enum EThreadState
	{
		E_NULL,
		E_READY,
		E_RUNNING
	};
public:
	CThread();
	virtual ~CThread();

	void start();
	void join();
	void cancel();

	EThreadState getState() { return m_state; }
	HANDLE& getHndThread() { return m_thdHandle; }

	static void sleep(uint32_t sec);
	static void msleep(uint32_t msec);
protected:
	virtual void _run() = 0;

private:
	static DWORD _threadRunFun(void* pVoid);
private:
	EThreadState m_state;
	//CHndThread m_thdHandle;
	HANDLE m_thdHandle;
	CMutex m_mutex;
};