#pragma once
#include "Tile.h"

enum class TileType : uint8_t;


class World
{
private:
	sf::Vector2i area;
	
	std::vector<TileType> maps;
	
public:
	World();
	World(int size_x, int size_y);

	void Draw(sf::RenderWindow& window, sf::Vector2i client_pos);

	sf::Vector2i GetArea() const;

	void LoadWorld();
	void StoreWorld();

};

