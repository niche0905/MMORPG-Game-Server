#pragma once

#include "Types.h"
#include "Container.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <windows.h>

#include <WinSock2.h>
#include <MSWSock.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

// �˰���
#include <random>
#include <chrono>
#include <ranges>
#include <algorithm>
#include <thread>

// �����
#include <iostream>
#include <fstream>
#include <filesystem>
#include <print>
