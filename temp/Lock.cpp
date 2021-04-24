#include <windows.h>
#include "Lock.h"
#include <assert.h>
#include <stdexcept>

namespace test
{

	
/*****************************************************************/
/*                Mutex						                     */
/*****************************************************************/
	Mutex::Mutex(const char *name): m_ownThreadId(0), m_mutex(0)
	{
		LPSECURITY_ATTRIBUTES lpMutexAttributes = 0;
		BOOL bInitialOwner = FALSE;
		
		/*if(g_isDebugEvent)
		{
			int nextID = IncDecEvent(true);
		
			char sName[MAX_PATH];
			sprintf( sName, "HJS%d", nextID);
			m_mutex = CreateMutex(lpMutexAttributes, bInitialOwner, sName);
		}
		else*/
		m_mutex = CreateMutex(lpMutexAttributes, bInitialOwner, name);
		if (!m_mutex)	//For Klocwork
		{
			throw std::runtime_error("Create Mutex fail!");
		}
	}

	bool Mutex::Lock(size_t timeOut)
	{
		DWORD dwSts = WaitForSingleObject(m_mutex, (DWORD)timeOut);
		switch (dwSts)
		{

		case WAIT_OBJECT_0:
			m_ownThreadId = GetCurrentThreadId();
			return true;
		case WAIT_TIMEOUT:
			return false;
		default:
			assert(!"Unknown Mutex status!");
			return false;
		}
	}

	bool Mutex::Unlock()
	{
		if (ReleaseMutex(m_mutex))
		{
			//m_ownThreadId = 0;
			return true;
		}
		else
		{
			//SYS_Assert(0);	//if unlock twice, then here SYS_Assert
			return false;
		}
	}

	Mutex::~Mutex()
	{
		CloseHandle(m_mutex);
		//IncDecEvent(false);
	}

/*****************************************************************/
/*                Lock						                     */
/*****************************************************************/

	Lock::Lock(Mutex& mutex)
		: m_mutex(mutex)
	{
		bool bSts = m_mutex.Lock(INFINITE);
		assert(bSts);
	}

	Lock::~Lock()
	{
		m_mutex.Unlock();
	}
}
