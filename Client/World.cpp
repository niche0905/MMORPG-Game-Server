#include "pch.h"
#include "World.h"
#include "TileManager.h"


World::World()
	: World{ 0, 0 }
{
}

// 월드 기본 생성자
World::World(int size_x, int size_y)
	: area{ size_x, size_y }
{
	maps.resize(size_x * size_y);

	for (auto& tile : maps)
		tile = TileType::grass;
}

// 월드 그리기 함수
void World::Draw(sf::RenderWindow& window, sf::Vector2i client_pos)
{
	int start_y = std::max(0, client_pos.y - TILE_LEN);
	int end_y = std::min(area.y - 1, client_pos.y + TILE_LEN);
	int start_x = std::max(0, client_pos.x - TILE_LEN);
	int end_x = std::min(area.x - 1, client_pos.x + TILE_LEN);

	for (int y = start_y; y <= end_y; ++y) {
		for (int x = start_x; x <= end_x; ++x) {
			TileManager::Instance().GetTile(maps[y * area.x + x]).Draw(window, x, y);
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
	std::cout << "Map Loading...\n";

	std::ifstream input_file{ "../Resource/map.bin", std::ifstream::binary };
	uint32 width, height;
	input_file.read(reinterpret_cast<char*>(&width), sizeof(uint32));
	input_file.read(reinterpret_cast<char*>(&height), sizeof(uint32));
	area = sf::Vector2i{ (int)width, (int)height };

	maps.resize(width * height);
	uint8 tile_info;
	for (auto& tile : maps) {
		input_file.read(reinterpret_cast<char*>(&tile_info), sizeof(uint8));
		tile = static_cast<TileType>(tile_info - 1);
	}
	input_file.close();

	std::cout << "Map Loading Done!\n";
}

// 월드 파일에 저장하기
void World::StoreWorld()
{

}
