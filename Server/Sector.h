#pragma once


class Sector
{
private:
	std::unordered_set<int64>		_clients;		// �ش� ������ �ִ� Ŭ���̾�Ʈ��
	std::unique_ptr<std::mutex>		_sector_mutex;	// �ش� ������ ���� ���ؽ�

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

