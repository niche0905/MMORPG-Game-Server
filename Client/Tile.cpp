#include "pch.h"
#include "Tile.h"
#include "TextureManager.h"


// 타일 기본 생성자
Tile::Tile()
{

}

// 타일 텍스쳐 key 기반 생성자
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
	
	sprite.setScale(TILE_SIZE / texture_size.x, TILE_SIZE / texture_size.y);
	sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);

	window.draw(sprite);

}
