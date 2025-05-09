#pragma once

#ifdef DEBUG
#pragma comment(lib, "Debug\\Core.lib")
#else
#pragma comment(lib, "Release\\Core.lib")
#endif

#include "CorePch.h"

#include "Packet.h"
