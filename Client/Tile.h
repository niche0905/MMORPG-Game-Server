#pragma once

// TODO : �뷮 �۰� int8�� ����ϱ�
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

