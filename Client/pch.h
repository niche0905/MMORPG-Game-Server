#pragma once

#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SFML_STATIC

#define LOOPBACK

#ifdef _DEBUG
#pragma comment(lib, "Debug\\Core.lib")
#else
#pragma comment(lib, "Release\\Core.lib")
#endif

// SFML 라이브러리
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "CorePch.h"
#include "ClientDefines.h"
#include "Object.h"

//#include "Packet.h"
#include "GlobalVariable.h"
