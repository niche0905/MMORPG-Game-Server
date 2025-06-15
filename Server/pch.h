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
#include <concurrent_priority_queue.h>
#include <sqlext.h>

#include "Position.h"
#include "Util.h"
#include "Packet.h"
#include "ServerDefines.h"
#include "Stat.h"
#include "ExpansionOverlapped.h"
#include "IocpCore.h"
#include "BaseSession.h"
#include "State.h"
#include "Bot.h"
#include "Monster.h"
#include "Session.h"
#include "DatabaseEvent.h"
#include "DatabaseManager.h"
#include "Sector.h"
#include "SectorManager.h"
#include "Event.h"
#include "WorldData.h"

#include "ServerCore.h"

#include "GlobalVariable.h"
