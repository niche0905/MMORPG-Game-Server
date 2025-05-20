#pragma once


struct Position
{
	int16 x, y;

	Position();
	Position(int16 ix, int16 iy);

	Position operator+(const Position& other) const;
	Position operator+=(const Position& other);
};

