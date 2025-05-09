#pragma once


class IocpCore
{
private:
	HANDLE _iocp_handle;

public:
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() const;

	void Init();

	void AddSocket(SOCKET socket, ULONG_PTR id);

	void Dispatch(DWORD& bytes_transferred, ULONG_PTR& key, LPOVERLAPPED& overlapped);

};

