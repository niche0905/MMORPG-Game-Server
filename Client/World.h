#pragma once
#include "Tile.h"

enum class TileType : uint8_t;


class World
{
private:
	sf::Vector2i area;
	// TODO : Tile 만들고 Tile을 가지게 2000*2000 (처음엔 작게 100*100)
	
	std::vector<TileType> maps;
	// 이 400백만 개를 그리는 것이 문제다
	
public:
	World(int size_x, int size_y);

	void Draw(sf::RenderWindow& window, sf::Vector2i client_pos);

	sf::Vector2i GetArea() const;

	void LoadWorld();
	void StoreWorld();

};

