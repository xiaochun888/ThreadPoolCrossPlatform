#include "CriticalSection.h"

CriticalSection::CriticalSection()
{
	#ifdef _WIN32
	InitializeCriticalSection(&m_lock);
	#else
	pthread_mutexattr_init(&m_attr);
	pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m_lock, &m_attr);
	#endif

};

CriticalSection::~CriticalSection()
{
	#ifdef _WIN32
	DeleteCriticalSection(&m_lock);
	#else
	pthread_mutex_destroy(&m_lock);
	pthread_mutexattr_destroy(&m_attr);
	#endif

};

void CriticalSection::enter()
{
	#ifdef _WIN32
	EnterCriticalSection(&m_lock);
	#else
	pthread_mutex_lock(&m_lock);
	#endif
};

void CriticalSection::leave()
{
	#ifdef _WIN32
	LeaveCriticalSection(&m_lock);
	#else
	pthread_mutex_unlock(&m_lock);
	#endif

};

/*If the critical section is successfully entered or the current thread already owns the critical section, 
 the return value is nonzero.If another thread already owns the critical section, the return value is zero.
*/
int CriticalSection::tryEnter(){
	#ifdef _WIN32
	return TryEnterCriticalSection(&m_lock);
	#else
	return !pthread_mutex_trylock(&m_lock);
	#endif
};
