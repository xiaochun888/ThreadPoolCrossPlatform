#include "ThreadPool.h"

void ThreadPool::popTask(void* groupe) {
	ThreadPool* thdGroupe = (ThreadPool*)groupe;

	while(!thdGroupe->m_tasks.empty()) {
		ThreadWorker::Task task = {0,0};
		thdGroupe->relock();
		if(!thdGroupe->m_tasks.empty()) {
			task = thdGroupe->m_tasks.front();
			thdGroupe->m_tasks.pop();
			thdGroupe->delock();
		} else {
			thdGroupe->delock();
			break;
		}

		if(task.proc) {
			task.proc(task.args);
		}
	}
}

ThreadPool::ThreadPool(int numberOfThread)
{
	for (int i = 0; i < numberOfThread; i++) {
		ThreadWorker* pThread = new ThreadWorker();
		ThreadWorker::Task task;
		task.args = this;
		task.proc = ThreadPool::popTask;
		pThread->task(task);
        m_threads.push_back(pThread);
	}
}

ThreadPool::~ThreadPool(void)
{
	//clear all
	relock();
	m_tasks = std::queue<ThreadWorker::Task>();
	delock();

	for (auto i = m_threads.begin(); i != m_threads.end(); ++i) {
		ThreadWorker * pThread = *i;
		delete pThread;
	}
}

void ThreadPool::load(ThreadWorker::Task const& task) {
	relock();
	m_tasks.push(task);
	delock();

	for (auto i = m_threads.begin(); i != m_threads.end(); ++i) {
		ThreadWorker * pThread = *i;
		if(!pThread->alive() || pThread->hung()) {
			pThread->start();
			if(!m_task.proc) m_bAlive = true;
			break;
		}
	}

	if(m_task.proc) ThreadWorker::start();
}

bool ThreadPool::suspend(bool waiting) {
	for (auto i = m_threads.begin(); i != m_threads.end(); ++i) {
		ThreadWorker * pThread = *i;
		pThread->suspend(waiting);
	}

	if(m_task.proc) return ThreadWorker::suspend(waiting);
	else {
		m_bHung = true;
		return m_bHung;
	}
}