#pragma once
#include <boost/noncopyable.hpp>

namespace test
{

	class Mutex: boost::noncopyable
	{
	public:
		explicit Mutex(const char *name);

		bool Lock(size_t timeOut);

		bool Unlock();
		~Mutex();
	private:
		void* m_mutex;
		unsigned int m_ownThreadId;
	};

	class  Lock
	{
	public:
		explicit Lock(Mutex& mutex);

		~Lock();
	private:
		Mutex &m_mutex;
	};


}
