#include "pch.h"
#include "Tile.h"
#include "TextureManager.h"


// 타일 기본 생성자
Tile::Tile(std::string texture_key)
{
	sprite.setTexture(TextureManager::Instance().GetTexture(texture_key));
}

// 타일 텍스쳐 생성자
Tile::Tile(sf::Texture& texture, int start_x, int start_y, int width, int height)
{
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(start_x, start_y, width, height));
}

// 타일 그리기
void Tile::Draw(sf::RenderWindow& window, int x, int y)
{
	sf::Vector2u texture_size = sprite.getTexture()->getSize();
	const double tile_size = 60.0;
	
	sprite.setScale(texture_size.x / tile_size, texture_size.y / tile_size);
	sprite.setPosition(x * tile_size, y * tile_size);

	window.draw(sprite);

}
