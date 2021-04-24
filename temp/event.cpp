#include "event.h"
#include <windows.h>
#include <assert.h>
#include <boost/foreach.hpp>

namespace GraphApi
{

/*explicit*/ Event::Event(bool bManualReset /*=TRUE*/, const char *name /*= NULL*/)
{
	LPSECURITY_ATTRIBUTES lpEventAttributes = 0;
	BOOL bInitialState = FALSE;
	m_handle = CreateEvent(lpEventAttributes, bManualReset, bInitialState, name);
	if (!m_handle)
	{
		throw std::runtime_error("Create Event Fail!");
	}
}

bool Event::set() const
{
	return !!::SetEvent(m_handle);
}

bool Event::reset() const
{
	return !!::ResetEvent(m_handle);
}
Event::Status Event::wait(long timeOut) const
{
	DWORD sts = WaitForSingleObject(m_handle, timeOut);
	switch (sts)
	{
	case WAIT_OBJECT_0:
		return Ok;
	case WAIT_TIMEOUT:
		return TimeOut;
	default:
		assert(!"Unknown even status!");
		return Unknown;
	}
}

Event::~Event()
{
	CloseHandle(m_handle);
	m_handle = 0;
	//IncDecEvent(false);
}

int waitMultiple(HANDLE *begin, HANDLE *end)
{
	DWORD id = WaitForMultipleObjects((DWORD)(end - begin), begin, FALSE, INFINITE);
	if (id < WAIT_OBJECT_0 + (end - begin))
	{
		int index = id - WAIT_OBJECT_0;
		return index;
	}
	else
		return -1;
}

int waitMultiple(std::vector<Event *> const &events)
{
	std::vector<HANDLE> handles;
	BOOST_FOREACH (Event *evt, events)
	{
		handles.push_back(evt->handle());
	}

	int index = waitMultiple(&handles[0], &handles[0] + handles.size());
	return index;
}

} // namespace GraphApi
