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
	bool IsBlock(int16 x, int16 y) const;
	bool IsBlock(Position pos) const;

	void LoadWorld();
};

