# libco相关数据结构

以下为libco中设计中的数据结构，按文件和功能整理

## co_routine.h
```c
struct stCoRoutineAttr_t
{
	int stack_size;
	stShareStack_t*  share_stack;
	stCoRoutineAttr_t()
	{
		stack_size = 128 * 1024;
		share_stack = NULL;
	}
}__attribute__ ((packed));

```

## co_routine.cpp

```cpp
struct stCoRoutineEnv_t
{
	stCoRoutine_t *pCallStack[ 128 ];
	int iCallStackSize;
	stCoEpoll_t *pEpoll;

	//for copy stack log lastco and nextco
	stCoRoutine_t* pending_co;
	stCoRoutine_t* occupy_co;
};


```


## co_routine_inner.h

```cpp
struct stStackMem_t
{
	stCoRoutine_t* occupy_co;
	int stack_size;
	char* stack_bp; //stack_buffer + stack_size
	char* stack_buffer;

};

struct stShareStack_t
{
	unsigned int alloc_idx;
	int stack_size;
	int count;
	stStackMem_t** stack_array;
};



struct stCoRoutine_t
{
	stCoRoutineEnv_t *env;
	pfn_co_routine_t pfn;
	void *arg;
	coctx_t ctx;

	char cStart;
	char cEnd;
	char cIsMain;
	char cEnableSysHook;
	char cIsShareStack;

	void *pvEnv;

	//char sRunStack[ 1024 * 128 ];``
	stStackMem_t* stack_mem;


	//save satck buffer while confilct on same stack_buffer;
	char* stack_sp;
	unsigned int save_size;
	char* save_buffer;

	stCoSpec_t aSpec[1024];

};

```
