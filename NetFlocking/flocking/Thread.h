#ifndef _THREAD_H_
#define _THREAD_H_

#ifdef __cplusplus
extern "C" {
#else	//__cplusplus
// struct foo
// foo bar; // C++ instance
// struct foo bar // C instance
typedef struct egpThread egpThread;
#endif	//__cplusplus

// Our definition of an acceptable function
// Any function that returns int and accepts any ptr param
typedef int(*egpThreadFunc)(void*);

struct egpThread {
	void* handle;
	unsigned int ID;

	int result;
	int isRunning;

	egpThreadFunc func;
	void* args;
};

int egpThreadCreate(egpThread* pThread_out, egpThreadFunc func, void* args);
int egpThreadTerminate(egpThread* pThread);

#ifdef __cplusplus
}
#else	//__cplusplus
#endif	//__cplusplus

#endif	// !_THREAD_H_