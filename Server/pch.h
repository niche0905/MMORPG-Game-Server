#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef DEBUG
#pragma comment(lib, "Debug\\Core.lib")
#else
#pragma comment(lib, "Release\\Core.lib")
#endif

#include "CorePch.h"

#include "Packet.h"
