#pragma once

#ifdef DEBUG
#pragma comment(lib, "Debug\\Core.lib")
#else
#pragma comment(lib, "Release\\Core.lib")
#endif

#include "CorePch.h"

// SFML 라이브러리
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Packet.h"
#include "GlobalVariable.h"
