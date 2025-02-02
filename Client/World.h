#pragma once
#include "Tile.h"


class World
{
private:
	sf::Vector2i area;
	// TODO : Tile ����� Tile�� ������ 2000*2000 (ó���� �۰� 100*100)
	
	std::vector<TileType> maps;
	
public:
	World(int size_x, int size_y);

	void Draw(sf::RenderWindow& window);

	void LoadWorld();
	void StoreWorld();

};

