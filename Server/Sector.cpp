#include "pch.h"
#include "Sector.h"

Sector::Sector()
	: _clients()
	, _sector_mutex(std::make_unique<std::mutex>())
{

}

Sector::~Sector()
{
	_clients.clear();
}

void Sector::AddClient(uint64 id)
{
	Lock();
	_clients.insert(id);
	Unlock();
}

void Sector::RemoveClient(uint64 id)
{
	Lock();
	auto it = _clients.find(id);
	if (it != _clients.end()) {
		_clients.erase(it);
	}
	Unlock();
}

std::unordered_set<uint64>& Sector::GetClientList()
{
	return _clients;
}

void Sector::Lock()
{
	_sector_mutex->lock();
}

void Sector::Unlock()
{
	_sector_mutex->unlock();
}
