#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#ifdef _DEBUG
#pragma comment(lib, "Debug\\Core.lib")
#else
#pragma comment(lib, "Release\\Core.lib")
#endif

#include "CorePch.h"

#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>
#include <sqlext.h>

#include "Util.h"
#include "Position.h"
#include "Packet.h"
#include "ServerDefines.h"
#include "Stat.h"
#include "IocpCore.h"
#include "ExpansionOverlapped.h"
#include "BaseSession.h"
#include "Bot.h"
#include "Session.h"
#include "DatabaseEvent.h"
#include "DatabaseManager.h"
#include "Sector.h"
#include "SectorManager.h"

#include "ServerCore.h"

#include "GlobalVariable.h"
