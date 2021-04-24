#pragma once
#include <vector>
#include <boost/noncopyable.hpp>

#define GRAPHAPI_EVENT_INFINITE (0xFFFFFFFF)

namespace GraphApi
{
class Event : boost::noncopyable
{
public:
	explicit Event(bool bManualReset = true, const char *name = NULL);
	~Event();

	bool IsValid() const
	{
		return m_handle != 0;
	}

	bool set() const;
	bool reset() const;

	enum Status
	{
		Ok = 0,
		TimeOut = -1,
		Unknown = -2
	};

	Status wait(long timeOut = GRAPHAPI_EVENT_INFINITE) const;

	void *handle() const { return m_handle; }

private:
	void *m_handle;
};

int waitMultiple(std::vector<Event *> const &events);

} // namespace GraphApi
