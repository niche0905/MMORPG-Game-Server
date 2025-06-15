#pragma once


class WorldData
{
private:
	Position area;

	std::vector<bool> maps;

public:
	WorldData();
	WorldData(int32 size_x, int32 size_y);

	Position GetArea() const;

	void LoadWorld();
};

