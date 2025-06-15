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

void WorldData::LoadWorld()
{
	std::cout << "Map Loading...\n";

	std::ifstream input_file{ "../Resource/map.bin", std::ifstream::binary };
	uint32 width, height;
	input_file.read(reinterpret_cast<char*>(&width), sizeof(uint32));
	input_file.read(reinterpret_cast<char*>(&height), sizeof(uint32));
	area = { static_cast<int16>(width), static_cast<int16>(height) };

	maps.resize(width * height);
	uint8 tile_info;
	for (int i = 0; i < maps.size(); ++i) {
		input_file.read(reinterpret_cast<char*>(&tile_info), sizeof(uint8));
		maps[i] = tile_info == 1 ? false : true;
	}
	input_file.close();

	std::cout << "Map Loading Done!\n";
}
