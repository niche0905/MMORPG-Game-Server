#pragma once


class SectorManager
{
private:
	std::vector<Sector>		_sectors;		// Sector ��ü�� ��� �ִ� ���� (2D �迭 ���·� ����ؾ� ��)
	int						_sector_size;	// ���� ũ��
	int						_sector_width;	// ���� ����

public:
	SectorManager(int sector_size = SECTOR_SIZE, uint16 width = MAX_WIDTH, uint16 height = MAX_HEIGHT);
	~SectorManager();

	void MoveClient(int64 id, short old_x, short old_y, short new_x, short new_y);
	void AddClient(int64 id, short x, short y);
	void RemoveClient(int64 id, short x, short y);

	void GetClientList(short x, short y, std::unordered_set<int64>& client_list);

private:
	int GetSectorIndex(short x, short y);

};

