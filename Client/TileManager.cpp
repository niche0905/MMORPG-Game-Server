#include "pch.h"
#include "TileManager.h"


// TileManager ½Ì±ÛÅæ
TileManager& TileManager::Instance()
{
	static TileManager instance;
	return instance;
}
