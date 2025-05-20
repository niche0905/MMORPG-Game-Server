#include "pch.h"
#include "IocpCore.h"


IocpCore::IocpCore()
{
	Init();
	std::cout << "IOCP Init Success\n";
}

IocpCore::~IocpCore()
{
	CloseHandle(_iocp_handle);
}

HANDLE IocpCore::GetHandle() const
{
	return _iocp_handle;
}

void IocpCore::Init()
{
	_iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if (NULL == _iocp_handle) {
		int err_code = GetLastError();
		PrintErrorMessage(L"IOCP handle Init", err_code);
		exit(-1);
	}
}

void IocpCore::AddSocket(SOCKET socket, ULONG_PTR id)
{
	HANDLE result = CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), _iocp_handle, id, 0);
	if (NULL == result) {
		int err_code = GetLastError();
		PrintErrorMessage(L"IOCP register socket", err_code);
	}
}

bool IocpCore::Dispatch(DWORD& bytes_transferred, ULONG_PTR& key, LPOVERLAPPED& overlapped)
{
	BOOL ret = GetQueuedCompletionStatus(_iocp_handle, &bytes_transferred, &key, &overlapped, INFINITE);
	if (FALSE == ret) {
		int err_code = GetLastError();
		PrintErrorMessage(L"IOCP get result", err_code);

		return false;
	}

	return true;
}
