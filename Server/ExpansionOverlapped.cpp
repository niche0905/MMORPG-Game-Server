#include "pch.h"
#include "ExpansionOverlapped.h"


ExpansionOverlapped::ExpansionOverlapped(IoOperation io_type = IoOperation::IO_NONE)
	: _wsa_overlapped{}
	, _wsabuf{}
	, _buffer{}
{
	ZeroMemory(&_wsa_overlapped, sizeof(_wsa_overlapped));
	SetOperation(io_type);
	_wsabuf[0].buf = reinterpret_cast<char*>(_buffer.data());
	_wsabuf[0].len = static_cast<ULONG>(_buffer.size());
}

void ExpansionOverlapped::Reset(int index)
{
	ZeroMemory(&_wsa_overlapped, sizeof(_wsa_overlapped));
	_wsabuf[0].buf = reinterpret_cast<char*>(_buffer.data() + index);
	_wsabuf[0].len = static_cast<ULONG>(_buffer.size() - index);
}

void ExpansionOverlapped::SetBuffer(void* packet, int size)
{
	_wsabuf[0].buf = reinterpret_cast<char*>(packet);
	_wsabuf[0].len = static_cast<ULONG>(size);
}

void ExpansionOverlapped::SetOperation(IoOperation operation)
{
	_operation = operation;
}

IoOperation ExpansionOverlapped::GetOperation() const
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
