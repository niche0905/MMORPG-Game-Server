#pragma once


// window에 관한 상수
constexpr int WINDOW_WIDTH{ 1600 };
constexpr int WINDOW_HEIGHT{ 900 };

constexpr int TILE_NUM{ 20 };
constexpr int TILE_LEN{ TILE_NUM / 2 };
constexpr double TILE_SIZE{ 40.0 };

constexpr double PLAYER_SIZE{ 35.0 };
constexpr double OBJECT_SIZE{ 40.0 };

inline constexpr const char* LOOPBACK_ADDRESS = "127.0.0.1";
