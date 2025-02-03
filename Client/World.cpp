#include "pch.h"
#include "World.h"
#include "TileManager.h"


// 월드 기본 생성자
World::World(int size_x, int size_y)
	: area{ size_x, size_y }
{
	maps.resize(size_x * size_y);

	for (auto& tile : maps)
		tile = TileType::grass;
}

// 월드 그리기 함수
void World::Draw(sf::RenderWindow& window)
{
	for (int y = 0; y < area.y; ++y) {
		for (int x = 0; x < area.x; ++x) {
			TileManager::Instance().GetTile(maps[y * area.y + x]).Draw(window, x, y);
		}
	}
}

sf::Vector2i World::GetArea() const
{
	return area;
}

// 월드 파일에서 불러오기
void World::LoadWorld()
{

}

// 월드 파일에 저장하기
void World::StoreWorld()
{

}
