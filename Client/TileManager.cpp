#include "pch.h"
#include "TileManager.h"

TileManager& TileManager::Instance()
{
	static TileManager instance;
	return instance;
}
