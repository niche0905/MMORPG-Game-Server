#pragma once

// SFML ���̺귯��
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
// ����2 ���� ���
#include <winsock2.h>
// ����2 Ȯ�� ���
#include <ws2tcpip.h>

 // ws2_32.lib ��ũ
#pragma comment(lib, "ws2_32")

// �ڷᱸ��
#include <list>
#include <vector>
#include <string>
#include <array>
#include <queue>
#include <unordered_map>
#include <memory>
#include <utility>

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
