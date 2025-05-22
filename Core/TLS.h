#pragma once

extern thread_local int32 thread_id;


void SetThreadId(int32 id);
int32 GetThreadId();
