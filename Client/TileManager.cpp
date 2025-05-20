﻿#include "pch.h"
#include "TileManager.h"


// TileManager 싱글톤
TileManager& TileManager::Instance()
{
	static TileManager instance;
	return instance;
}

void TileManager::AddTile(TileType tile_type, std::string texture_key)
{
	tiles.insert(std::make_pair(tile_type, Tile{texture_key}));
}

Tile& TileManager::GetTile(TileType tile_type)
{
	if (tiles.find(tile_type) != tiles.end())
		return tiles[tile_type];

	// 문제 상황!!
	std::wcout << L"타일 오류!!\n";
	exit(-1);
}
