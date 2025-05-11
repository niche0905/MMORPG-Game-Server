#pragma once


class Sector
{
private:
	std::unordered_set<int64>		_clients;		// 해당 구역에 있는 클라이언트들
	std::unique_ptr<std::mutex>		_sector_mutex;	// 해당 구역에 대한 뮤텍스

public:
	Sector();
	~Sector();

	Sector(const Sector& other) :
		_clients{ other._clients }
		, _sector_mutex{ std::make_unique<std::mutex>() }
	{

	}

	Sector operator=(const Sector& other) {
		if (this != &other) {
			_clients = other._clients;
		}
		return *this;
	}

	Sector(Sector&& other) noexcept = default;
	Sector& operator=(Sector&& other) noexcept = default;

	void AddClient(int64 id);
	void RemoveClient(int64 id);

	void Lock();
	void Unlock();

};

