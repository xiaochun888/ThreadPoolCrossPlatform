// ThreadPoolUT.cpp : définit le point d'entrée pour l'application console.
//
#include "../stdafx.h"
#include "..\CppUnitTestExpress.h"
#include "..\Sources\ThreadPool.h"
#include "..\Sources\CriticalSection.h"

CriticalSection _section;

void print(void* data) {
	CriticalSection::Guard guard(_section);
	_section.enter();
	int* value = (int*)data;
	UnitTest::dprintf("%d\n", *value);
	_section.leave();
}

class TestThreadPool : public Unit<TestThreadPool>
{ 
public:
	void Test()
	{
		ThreadPool pool(10);

		int data[20];
		for(int i = 0; i < 20; i++){
			data[i] = i;

			ThreadWorker::Task task;
			task.args = &data[i];
			task.proc = print;

			pool.load(task);
		}

		_assert(pool.alive(), "Should be started.");
		pool.suspend();
	}
};
