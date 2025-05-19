#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef DEBUG
#pragma comment(lib, "Debug\\Core.lib")
#else
#pragma comment(lib, "Release\\Core.lib")
#endif

#include "CorePch.h"

#include <concurrent_unordered_map.h>
#include <atomic>
#include <mutex>

#include "Util.h"
#include "Packet.h"
#include "ServerDefines.h"
#include "Stat.h"
#include "IocpCore.h"
#include "ExpansionOverlapped.h"
#include "Session.h"
#include "ServerCore.h"
#include "Sector.h"

#include "GlobalVariable.h"
