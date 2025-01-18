#pragma once

// TODO : 용량 작게 int8로 사용하기
enum TileType : uint8_t
{

};


class Tile
{
private:
	sf::Sprite sprite;
	TileType type;

public:
	Tile(TileType tile_type);
	Tile(TileType tile_type, sf::Texture& texture, int start_x, int start_y, int width, int height);

	void Draw(sf::RenderWindow& window, int x, int y);

};

