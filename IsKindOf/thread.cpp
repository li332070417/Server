#include "thread.h"

CThread::CThread() : m_state(E_NULL), m_thdHandle(NULL)
{

}

CThread::~CThread()
{

}

void CThread::start()
{
	if (m_state != E_NULL)
	{
		return;
	}
	m_state = E_READY;
	m_thdHandle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_threadRunFun, this, 0, NULL);
	if (m_thdHandle == NULL)
	{
		return;	
	}
	while (m_state != E_READY)
	{
		msleep(1);
		break;
	}
}

void CThread::join()
{
	m_mutex.Lock();
	if (m_state == E_RUNNING || m_thdHandle != nullptr)
	{
		::WaitForSingleObject(m_thdHandle, INFINITE);
	}
	m_mutex.Unlock();
}

DWORD CThread::_threadRunFun(void* pVoid)
{
	CThread* pThread = (CThread*)pVoid;
	if (pThread == nullptr)
	{
		return 0;
	}
	pThread->m_mutex.Lock();
	pThread->m_state = E_RUNNING;
	pThread->m_mutex.Unlock();

	pThread->_run();

	pThread->m_state = E_NULL;

	return 0;
}