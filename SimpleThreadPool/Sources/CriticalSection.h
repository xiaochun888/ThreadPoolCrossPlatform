#pragma once
#ifndef _CRITICAL_SECTION_H_
#define _CRITICAL_SECTION_H_

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

class CriticalSection
{
public:

    CriticalSection();
    ~CriticalSection();
    void enter();
    void leave();

	/*If the critical section is successfully entered or the current thread already owns the critical section, 
	 the return value is nonzero.If another thread already owns the critical section, the return value is zero.
	*/
    int tryEnter();

    class Guard
    {
		public:
			Guard(CriticalSection& cs) : _cs(cs) { _cs.enter(); }
			~Guard(){ _cs.leave(); }
			Guard& operator=(const Guard& g) 
			{
				if(this != &g) _cs = g._cs;
				return *this;
			}
		private:
			CriticalSection& _cs;
    };

private:
	#ifdef _WIN32
	CRITICAL_SECTION m_lock;
	#else
	pthread_mutexattr_t m_attr;
	pthread_mutex_t	m_lock;
	#endif
};

#endif // _CRITICAL_SECTION_H_
