#include "pch.h"
#include "SectorManager.h"


SectorManager::SectorManager(int32 sector_size, uint16 width, uint16 height)
	: _sector_size(sector_size)
{
	_sector_width = ((width + _sector_size - 1) / _sector_size);
	int32 sector_num = _sector_width * ((height + _sector_size - 1) / _sector_size);
	_sectors.resize(sector_num);
}

SectorManager::~SectorManager()
{
	_sectors.clear();
}

void SectorManager::MoveClient(uint64 id, short old_x, short old_y, short new_x, short new_y)
{
	int32 old_index = GetSectorIndex(old_x, old_y);
	int32 new_index = GetSectorIndex(new_x, new_y);

	if (old_index == new_index) return;

	int32 first = std::min<int32>(old_index, new_index);
	int32 second = std::max<int32>(old_index, new_index);

	_sectors[first].Lock();
	_sectors[second].Lock();

	_sectors[old_index].RemoveClientOnly(id);
	_sectors[new_index].AddClientOnly(id);

	_sectors[first].Unlock();
	_sectors[second].Unlock();
}

void SectorManager::AddClient(uint64 id, short x, short y)
{
	int32 index = GetSectorIndex(x, y);
	_sectors[index].AddClient(id);
}

void SectorManager::AddClient(uint64 id, Position pos)
{
	int32 index = GetSectorIndex(pos.x, pos.y);
	_sectors[index].AddClient(id);
}

void SectorManager::RemoveClient(uint64 id, short x, short y)
{
	int32 index = GetSectorIndex(x, y);
	_sectors[index].RemoveClient(id);
}

void SectorManager::GetClientList(short x, short y, std::unordered_set<uint64>& client_list)
{
	std::set<int> lock_sectors;

	for (int i = 0; i < 4; ++i) {
		int32 sector_x = std::clamp((x + vx[i]), 0, static_cast<int32>(MAX_WIDTH) - 1);
		int32 sector_y = std::clamp((y + vy[i]), 0, static_cast<int32>(MAX_HEIGHT) - 1);
		int32 sector_id = GetSectorIndex(sector_x, sector_y);
		lock_sectors.insert(sector_id);
	}

	for (auto& sector_id : lock_sectors) {
		_sectors[sector_id].Lock();
	}

	for (auto& sector_id : lock_sectors) {
		auto user_list_in_sector = _sectors[sector_id].GetClientList();
		_sectors[sector_id].Unlock();
		for (auto& user : user_list_in_sector) {
			client_list.insert(user);
		}
	}
}

int32 SectorManager::GetSectorIndex(short x, short y)
{
	return (x / _sector_size) + ((y / _sector_size) * _sector_width);
}
