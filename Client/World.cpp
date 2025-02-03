#include "pch.h"
#include "World.h"
#include "TileManager.h"


// ���� �⺻ ������
World::World(int size_x, int size_y)
	: area{ size_x, size_y }
{
	maps.resize(size_x * size_y);

	for (auto& tile : maps)
		tile = TileType::grass;
}

// ���� �׸��� �Լ�
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

// ���� ���Ͽ��� �ҷ�����
void World::LoadWorld()
{

}

// ���� ���Ͽ� �����ϱ�
void World::StoreWorld()
{

}
