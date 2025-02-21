#pragma once
#include "Tile.h"


class TileManager
{
private:
	std::unordered_map<TileType, Tile> tiles;

	TileManager() = default;
	~TileManager() = default;
	TileManager(const TileManager&) = delete;
	TileManager& operator=(const TileManager&) = delete;

public:
	static TileManager& Instance();

	void AddTile(TileType tile_type, std::string texture_key);

	Tile& GetTile(TileType tile_type);

};

