#include <Windows.h>
#include <stdio.h>

#include "Thread.h"

// ---------------------------------------------------------------------

long __stdcall egpThreadLaunchInternal(egpThread* pParam) {

	pParam->isRunning = 1;
	pParam->result = pParam->func(pParam->args);
	pParam->isRunning = 0;

	return 0;
}

// ---------------------------------------------------------------------

int egpThreadCreate(egpThread* pThread_out, egpThreadFunc func, void* args) {
	if (pThread_out && !pThread_out->handle && func) {
		// Reset thread struct
		pThread_out->result = 0;
		pThread_out->isRunning = 0;
		pThread_out->func = func;
		pThread_out->args = args;

		// Launch thread
		pThread_out->handle = CreateThread(0, 0, egpThreadLaunchInternal, pThread_out, 0, &pThread_out->ID);

		return (pThread_out->ID);
	}

	return 0;
}

int egpThreadTerminate(egpThread* pThread) {
	if (pThread && pThread->handle) {

		int success = TerminateThread(pThread->handle, -1);
		if (success == 1) {
			success = CloseHandle(pThread->handle);
			if (success == 1) {
				pThread->handle = 0;

				return 1;
			}
		}

		return 1;
	}

	return 0;
}