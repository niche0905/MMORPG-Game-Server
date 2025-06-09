#pragma once

class ExpansionOverlapped
{
private:
	WSAOVERLAPPED						_wsa_overlapped;
	WSABUF								_wsabuf[1];
	std::array<BYTE, BUF_SIZE>			_buffer;
	OverOperation							_operation;

public:
	ExpansionOverlapped(OverOperation io_type = OverOperation::OVER_NONE);

	~ExpansionOverlapped() = default;

	void Reset(int index);

	void SetBuffer(void* packet, int size);

	void SetOperation(OverOperation operation);
	
	void CopyToBuffer(void* data, int size);

	OverOperation GetOperation() const;

	WSAOVERLAPPED* GetOverlapped();

	WSABUF* GetWSABuf();

	BYTE* GetBuffer();

};

using ExOver = ExpansionOverlapped;
