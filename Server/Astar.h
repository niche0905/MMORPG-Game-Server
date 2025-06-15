#pragma once
#include <functional>


class Astar
{
public:
	static std::vector<Position> FindPath(Position start, Position goal, std::function<bool(const Position&)> IsBlocked);

private:
	static int32 Heuristic(const Position& a, const Position& b);

};

