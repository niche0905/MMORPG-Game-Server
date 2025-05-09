#pragma once

class ExpansionOverlapped
{
private:
	WSAOVERLAPPED						_wsa_overlapped;
	WSABUF								_wsabuf[1];
	std::array<BYTE, 1024/*buff_size*/>	_buffer;	// TODO : BUF_SIZE 정의 후 변경하기
	IoOperation							_operation;

public:
	ExpansionOverlapped(IoOperation io_type = IoOperation::IO_NONE)
		: _wsa_overlapped{}
		, _wsabuf{}
		, _buffer{}
	{
		ZeroMemory(&_wsa_overlapped, sizeof(_wsa_overlapped));
		SetOperation(io_type);
		_wsabuf[0].buf = reinterpret_cast<char*>(_buffer.data());
		_wsabuf[0].len = static_cast<ULONG>(_buffer.size());
	}

	~ExpansionOverlapped() = default;

	void Reset(int index)
	{
		ZeroMemory(&_wsa_overlapped, sizeof(_wsa_overlapped));
		_wsabuf[0].buf = reinterpret_cast<char*>(_buffer.data() + index);
		_wsabuf[0].len = static_cast<ULONG>(_buffer.size() - index);
	}

	void SetBuffer(void* packet, int size)
	{
		_wsabuf[0].buf = reinterpret_cast<char*>(packet);
		_wsabuf[0].len = static_cast<ULONG>(size);
	}

	void SetOperation(IoOperation operation)
	{
		_operation = operation;
	}

	IoOperation GetOperation() const
	{
		return _operation;
	}

	WSAOVERLAPPED* GetOverlapped()
	{
		return &_wsa_overlapped;
	}

	WSABUF* GetWSABuf()
	{
		return _wsabuf;
	}

	BYTE* GetBuffer()
	{
		return _buffer.data();
	}

};

using ExOver = ExpansionOverlapped;
