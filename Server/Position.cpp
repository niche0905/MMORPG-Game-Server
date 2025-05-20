#include "pch.h"
#include "Position.h"


Position::Position()
	: Position{ 0, 0 }
{

}

Position::Position(int16 ix, int16 iy)
	: x{ ix }, y{ iy }
{

}

Position Position::operator+(const Position& other) const
{
	return {
		static_cast<int16>(x + other.x),
		static_cast<int16>(y + other.y)
	};
}

Position Position::operator+=(const Position& other)
{
	x += other.x;
	y += other.y;
	return *this;
}
