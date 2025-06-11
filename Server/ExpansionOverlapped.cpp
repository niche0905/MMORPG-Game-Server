#include "pch.h"
#include "ExpansionOverlapped.h"


ExpansionOverlapped::ExpansionOverlapped(OverOperation io_type)
	: _wsa_overlapped{}
	, _wsabuf{}
	, _buffer{}
{
	ZeroMemory(&_wsa_overlapped, sizeof(_wsa_overlapped));
	SetOperation(io_type);
	_wsabuf[0].buf = reinterpret_cast<char*>(_buffer.data());
	_wsabuf[0].len = static_cast<ULONG>(_buffer.size());
}

void ExpansionOverlapped::Reset(int32 index)
{
	ZeroMemory(&_wsa_overlapped, sizeof(_wsa_overlapped));
	_wsabuf[0].buf = reinterpret_cast<char*>(_buffer.data() + index);
	_wsabuf[0].len = static_cast<ULONG>(_buffer.size() - index);
}

void ExpansionOverlapped::SetBuffer(void* packet, int32 size)
{
	memcpy(_buffer.data(), packet, size);

	_wsabuf[0].buf = reinterpret_cast<char*>(_buffer.data());
	_wsabuf[0].len = static_cast<ULONG>(size);
}

void ExpansionOverlapped::SetOperation(OverOperation operation)
{
	_operation = operation;
}

void ExpansionOverlapped::CopyToBuffer(void* data, int32 size)
{
	memcpy(_buffer.data(), data, size);
}

OverOperation ExpansionOverlapped::GetOperation() const
{
	return _operation;
}

WSAOVERLAPPED* ExpansionOverlapped::GetOverlapped()
{
	return &_wsa_overlapped;
}

WSABUF* ExpansionOverlapped::GetWSABuf()
{
	return _wsabuf;
}

BYTE* ExpansionOverlapped::GetBuffer()
{
	return _buffer.data();
}
