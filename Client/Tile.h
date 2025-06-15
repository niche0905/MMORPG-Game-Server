#pragma once
#include "Game.h"



enum class TileType : uint8
{
	grass,
	water,
	house,
	tree
};


class Tile
{
private:
	sf::Sprite sprite;

public:
	Tile();
	Tile(std::string texture_key);
	Tile(sf::Texture& texture, int start_x, int start_y, int width, int height);

	void Draw(sf::RenderWindow& window, int x, int y);

};

