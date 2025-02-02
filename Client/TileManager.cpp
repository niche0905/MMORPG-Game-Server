#include "pch.h"
#include "TileManager.h"


// TileManager ΩÃ±€≈Ê
TileManager& TileManager::Instance()
{
	static TileManager instance;
	return instance;
}

void TileManager::Add(TileType tile_type, std::string texture_key)
{
	tiles.insert(std::make_pair(tile_type, Tile{texture_key}));
}

Tile& TileManager::GetTile(TileType tile_type)
{
	if (tiles.find(tile_type) != tiles.end())
		return tiles[tile_type];

	// πÆ¡¶ ªÛ»≤!!
	exit(-1);
}
