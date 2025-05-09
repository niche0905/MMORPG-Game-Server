#pragma once

class ExpansionOverlapped
{
private:
	WSAOVERLAPPED						_wsa_overlapped;
	WSABUF								_wsabuf[1];
	std::array<BYTE, 1024/*buff_size*/>	_buffer;	// TODO : BUF_SIZE 정의 후 변경하기
	IoOperation							_operation;

public:
	ExpansionOverlapped(IoOperation io_type = IoOperation::IO_NONE);

	~ExpansionOverlapped() = default;

	void Reset(int index);

	void SetBuffer(void* packet, int size);

	void SetOperation(IoOperation operation);

	IoOperation GetOperation() const;

	WSAOVERLAPPED* GetOverlapped();

	WSABUF* GetWSABuf();

	BYTE* GetBuffer();

};

using ExOver = ExpansionOverlapped;
