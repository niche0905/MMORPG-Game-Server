#include "pch.h"
#include "TLS.h"

thread_local int32 thread_id = -1;


void SetThreadId(int32 id)
{
	thread_id = id;
}

int32 GetThreadId()
{
	return thread_id;
}
