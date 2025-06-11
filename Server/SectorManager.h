#pragma once


class SectorManager
{
private:
	std::vector<Sector>		_sectors;		// Sector 객체를 담고 있는 벡터 (2D 배열 형태로 사용해야 함)
	int						_sector_size;	// 섹터 크기
	int						_sector_width;	// 섹터 넓이

public:
	SectorManager(int32 sector_size = SECTOR_SIZE, uint16 width = MAX_WIDTH, uint16 height = MAX_HEIGHT);
	~SectorManager();

	void MoveClient(uint64 id, short old_x, short old_y, short new_x, short new_y);
	void AddClient(uint64 id, short x, short y);
	void AddClient(uint64 id, Position pos);
	void RemoveClient(uint64 id, short x, short y);

	void GetClientList(short x, short y, std::unordered_set<uint64>& client_list);

private:
	int32 GetSectorIndex(short x, short y);

};

