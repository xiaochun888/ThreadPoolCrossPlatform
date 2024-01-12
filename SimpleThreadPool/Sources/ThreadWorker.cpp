#include "ThreadWorker.h"

#ifdef _WIN32
unsigned long __stdcall ThreadWorker::m_threadRoutine( ThreadWorker * worker )
#else
void* ThreadWorker::m_threadRoutine( ThreadWorker * worker )
#endif
{
	while(worker->m_bAlive)
	{
		if(!worker->m_bHung)
		{
			worker->relock();
			if(worker->m_task.proc)
			{
				worker->m_task.proc(worker->m_task.args);
			}
			worker->delock();
		}

		if(!worker->m_bAlive) break;
		else worker->suspend(false);
	}

	return 0;
};

ThreadWorker::ThreadWorker(void)
{
	m_task.args = 0;
	m_task.proc = 0;

	/* Internal */
	m_bAlive = false;
	m_bHung = false;

	#ifdef _WIN32
	InitializeCriticalSection(&m_mutex);
	#else
	pthread_mutexattr_init(&m_attr);
	pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&m_mutex, &m_attr);
	pthread_cond_init(&(m_cond), NULL);
	#endif

}


ThreadWorker::~ThreadWorker(void)
{
	stop();

	#ifdef _WIN32
	DeleteCriticalSection(&m_mutex);
	#else
	pthread_cond_destroy(&m_cond);
	pthread_mutex_destroy(&m_mutex);
	pthread_mutexattr_destroy(&m_attr);
	#endif
}

bool ThreadWorker::stop() {
	if(m_bAlive)
	{
		m_bAlive = false;
		resume();

		#ifdef _WIN32
		WaitForSingleObject(m_hthread, INFINITE);
		CloseHandle(m_hthread);
		#else
		pthread_join(m_threadid, NULL);
		#endif
	}
	return !m_bAlive;
}

bool ThreadWorker::start() {
	if(!m_bAlive)
	{
		m_bAlive = true;

		#ifdef _WIN32
		m_hthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)m_threadRoutine, (void*)this, 0, &m_threadid);
		#else
		pthread_create(&m_threadid, NULL, (void*(*)(void*))m_threadRoutine, (void*)this);
		#endif
	}
	else
	{
		resume();
	}
	return m_bAlive;

}

bool ThreadWorker::resume() {
	if(m_bHung)
	{
		m_bHung = false;

		#ifdef _WIN32
		EnterCriticalSection(&m_mutex);
		ResumeThread(m_hthread);
		LeaveCriticalSection(&m_mutex);
		#else
		pthread_mutex_lock(&m_mutex);
		pthread_cond_broadcast(&m_cond);
		pthread_mutex_unlock(&m_mutex);
		#endif
	}
	return !m_bHung;
}

bool ThreadWorker::suspend(bool waiting) {
	if(m_bAlive && !m_bHung)
	{
		m_bHung = true;

		if(waiting) {
			relock();
			delock();
		}

		#ifdef _WIN32
		SuspendThread(m_hthread);
		#else
		pthread_mutex_lock(&m_mutex);
		pthread_cond_wait(&m_cond, &m_mutex);
		pthread_mutex_unlock(&m_mutex);
		#endif
	}
	return m_bHung;
}

void ThreadWorker::relock() {
	#ifdef _WIN32
	EnterCriticalSection(&m_mutex);
	#else
	pthread_mutex_lock(&m_mutex);
	#endif
}

void ThreadWorker::delock() {
	#ifdef _WIN32
	LeaveCriticalSection(&m_mutex);
	#else
	pthread_mutex_unlock(&m_mutex);
	#endif
}

