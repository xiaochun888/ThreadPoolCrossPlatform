# Thread pool cross platform
 A enough simple and cross-platform thread pool for C++.
 It is enough to copy le folder ThreadPoolCrossPlatform/SimpleThreadPool/Sources into your application. 
 
## Main features:

It is designed for user to easily modify so as to integrate into your project.

1. No concept to learn, no graphic interface, no extrernal library.
2. Extended easily and integrated easily into an application.

## A minimal example:

The below code snippet demonstrates the least amount of code to write an multi-threads console application:

```
#include ".\Sources\ThreadPool.h"
#include ".\Sources\CriticalSection.h"

CriticalSection _section;

void print(void* data) {
	CriticalSection::Guard guard(_section);
	
	_section.enter();
	int* value = (int*)data;
	printf("%d\n", *value);
	_section.leave();
}

int _tmain(int argc, _TCHAR* argv[])
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

	pool.suspend();

	return 0;
}
```
