#pragma once

// SFML 라이브러리
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
// 윈속2 메인 헤더
#include <winsock2.h>
// 윈속2 확장 헤더
#include <ws2tcpip.h>

 // ws2_32.lib 링크
#pragma comment(lib, "ws2_32")

// 자료구조
#include <list>
#include <vector>
#include <string>
#include <array>
#include <queue>
#include <unordered_map>
#include <memory>
#include <utility>

// 알고리즘
#include <random>
#include <chrono>
#include <ranges>
#include <algorithm>
#include <thread>

// 입출력
#include <iostream>
#include <fstream>
#include <filesystem>
#include <print>

// type define
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;


#include "Packet.h"
#include "GlobalVariable.h"
