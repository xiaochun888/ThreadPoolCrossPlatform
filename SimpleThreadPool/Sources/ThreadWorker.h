#pragma once
#ifndef _THREAD_WORKER_H_
#define _THREAD_WORKER_H_

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif


class ThreadWorker
{
public:
	/**Initialize */
	ThreadWorker(void);
	~ThreadWorker(void);

	typedef struct Task {
		void (*proc)(void*);
		void *args;
	} Task;

	/** Check if thread is alive. */
	bool alive() { return m_bAlive; }

	/** Check if thread is hung. */
	bool hung() { return m_bHung; }

	/**Create and start thread. */
	bool start();

	/** Stop thread*/
	bool stop();

	/**Restart thread.*/
	bool resume();

	/**Wait for task finished par default. */
	bool suspend(bool waiting = true);

	/**Lock if not locked. */
	void relock();

	/**Delock. */
	void delock();

	/**Set task with args and procedure. */
	void task(ThreadWorker::Task const& task) { m_task = task; }

	/**Get task. */
	ThreadWorker::Task& task() { return m_task; }

protected:

	#ifdef _WIN32
	HANDLE	m_hthread;
	DWORD	m_threadid;
	#else
	pthread_t	m_threadid;
	#endif

	#ifdef _WIN32
	CRITICAL_SECTION m_mutex;
	#else
	pthread_mutexattr_t m_attr;
	pthread_cond_t		m_cond;
	pthread_mutex_t		m_mutex;
	#endif

	#ifdef _WIN32
	static unsigned long __stdcall m_threadRoutine( ThreadWorker * worker );
	#else
	static void* m_threadRoutine( ThreadWorker * worker );
	#endif

	Task		m_task;
	bool		m_bAlive;
	bool		m_bHung;
};

#endif //_THREAD_WORKER_H_

