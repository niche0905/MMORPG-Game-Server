#include "pch.h"
#include "Tile.h"


// Ÿ�� �⺻ ������
Tile::Tile(TileType tile_type)
	:type{ tile_type }
{

}

// Ÿ�� �ؽ��� ������
Tile::Tile(TileType tile_type, sf::Texture& texture, int start_x, int start_y, int width, int height)
	:type{ tile_type }
{
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(start_x, start_y, width, height));
}

// Ÿ�� �׸���
void Tile::Draw(sf::RenderWindow& window, int x, int y)
{
	// TODO : ���� x, y�� ������� window�� �׸���
}
