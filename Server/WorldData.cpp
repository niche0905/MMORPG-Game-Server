#include "pch.h"
#include "WorldData.h"


WorldData::WorldData()
	: WorldData{ 0, 0 }
{

}

WorldData::WorldData(int32 size_x, int32 size_y)
	: area{ static_cast<int16>(size_x), static_cast<int16>(size_y) }
{
	maps.resize(area.x * area.y);
}

Position WorldData::GetArea() const
{
	return area;
}

bool WorldData::IsBlock(int16 x, int16 y) const
{
	int32 index = y * area.x + x;
	if (index < 0 or index >= maps.size()) return true;

	return maps[index];
}

bool WorldData::IsBlock(Position pos) const
{
	int32 index = pos.y * area.x + pos.x;
	if (index < 0 or index >= maps.size()) return true;

	return maps[index];
}

void WorldData::LoadWorld(const std::filesystem::path& path)
{
	std::cout << "Map Loading...\n";

	std::ifstream input_file{ path, std::ifstream::binary };
	if (not input_file.is_open()) {
		throw std::runtime_error("Failed to open map file: " + path.string());
	}
	uint32 width, height;
	input_file.read(reinterpret_cast<char*>(&width), sizeof(uint32));
	input_file.read(reinterpret_cast<char*>(&height), sizeof(uint32));
	if (not input_file or width == 0 or height == 0) {
		throw std::runtime_error("Invalid map header: " + path.string());
	}
	area = { static_cast<int16>(width), static_cast<int16>(height) };

	maps.resize(width * height);
	uint8 tile_info;
	for (int i = 0; i < maps.size(); ++i) {
		input_file.read(reinterpret_cast<char*>(&tile_info), sizeof(uint8));
		if (not input_file) {
			throw std::runtime_error("Invalid map data: " + path.string());
		}
		maps[i] = tile_info == 1 ? false : true;
	}
	input_file.close();

	std::cout << "Map Loading Done!\n";
}
