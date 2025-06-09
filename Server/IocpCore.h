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

	bool Dispatch(DWORD& bytes_transferred, ULONG_PTR& key, LPOVERLAPPED& overlapped);

	void AddTask(uint64 id, ExOver* ex_over);

};

