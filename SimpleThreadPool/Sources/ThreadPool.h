#pragma once
#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include "ThreadWorker.h"
#include <queue>

class ThreadPool : public ThreadWorker
{
public:
	ThreadPool(int numberOfThread);
	virtual ~ThreadPool(void);

	/**Wait for all the tasks finished par default.*/
	bool suspend(bool waiting = true);

	/**Load a task into pool.*/
	void load(ThreadWorker::Task const& task);
protected:
	static void popTask(void*);
private:
	std::vector<ThreadWorker*> m_threads;
	std::queue<ThreadWorker::Task> m_tasks;
};

#endif // _THREAD_POOL_H_
