#include "pch.h"
#include "Tile.h"
#include "TextureManager.h"


// Ÿ�� �⺻ ������
Tile::Tile(std::string texture_key)
{
	sprite.setTexture(TextureManager::Instance().GetTexture(texture_key));
}

// Ÿ�� �ؽ��� ������
Tile::Tile(sf::Texture& texture, int start_x, int start_y, int width, int height)
{
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(start_x, start_y, width, height));
}

// Ÿ�� �׸���
void Tile::Draw(sf::RenderWindow& window, int x, int y)
{
	sf::Vector2u texture_size = sprite.getTexture()->getSize();
	const double tile_size = 60.0;
	
	sprite.setScale(texture_size.x / tile_size, texture_size.y / tile_size);
	sprite.setPosition(x * tile_size, y * tile_size);

	window.draw(sprite);

}
