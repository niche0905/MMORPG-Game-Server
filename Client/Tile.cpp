#include "pch.h"
#include "Tile.h"


// 타일 기본 생성자
Tile::Tile(TileType tile_type)
	:type{ tile_type }
{

}

// 타일 텍스쳐 생성자
Tile::Tile(TileType tile_type, sf::Texture& texture, int start_x, int start_y, int width, int height)
	:type{ tile_type }
{
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(start_x, start_y, width, height));
}

// 타일 그리기
void Tile::Draw(sf::RenderWindow& window, int x, int y)
{
	// TODO : 받은 x, y를 기반으로 window에 그리기
}
