#pragma once
#include "Tile.h"


class World
{
private:
	sf::Vector2i area;
	// TODO : Tile 만들고 Tile을 가지게 2000*2000 (처음엔 작게 100*100)
	
	std::vector<TileType> maps;
	
public:
	World(int size_x, int size_y);

	void Draw(sf::RenderWindow& window);

	void LoadWorld();
	void StoreWorld();

};

