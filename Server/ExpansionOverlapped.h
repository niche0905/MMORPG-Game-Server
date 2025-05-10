#pragma once

class ExpansionOverlapped
{
private:
	WSAOVERLAPPED						_wsa_overlapped;
	WSABUF								_wsabuf[1];
	std::array<BYTE, BUF_SIZE>			_buffer;
	IoOperation							_operation;

public:
	ExpansionOverlapped(IoOperation io_type = IoOperation::IO_NONE);

	~ExpansionOverlapped() = default;

	void Reset(int index);

	void SetBuffer(void* packet, int size);

	void SetOperation(IoOperation operation);
	
	void CopyToBuffer(void* data, int size);

	IoOperation GetOperation() const;

	WSAOVERLAPPED* GetOverlapped();

	WSABUF* GetWSABuf();

	BYTE* GetBuffer();

};

using ExOver = ExpansionOverlapped;
