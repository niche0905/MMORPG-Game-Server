#include "pch.h"
#include "IocpCore.h"


IocpCore::IocpCore()
{
	Init();
}

IocpCore::~IocpCore()
{

}

HANDLE IocpCore::GetHandle() const
{
	return _iocp_handle;
}

void IocpCore::Init()
{
	_iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if (NULL == _iocp_handle) {
		// TEMP : 에러 메시지 출력 (임시)
		int err_code = GetLastError();
		std::cout << "CreateIoCompletionPort Error: " << err_code << std::endl;
		exit(-1);
	}
}

void IocpCore::AddSocket(SOCKET socket, ULONG_PTR id)
{
	HANDLE result = CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), _iocp_handle, id, 0);
	if (NULL == result) {
		// TEMP : 에러 메시지 출력 (임시)
		int err_code = GetLastError();
		std::cout << "IOCP register socket Error: " << err_code << std::endl;
	}
}

void IocpCore::Dispatch(DWORD& bytes_transferred, ULONG_PTR& key, LPOVERLAPPED& overlapped)
{
	BOOL ret = GetQueuedCompletionStatus(_iocp_handle, &bytes_transferred, &key, &overlapped, INFINITE);
	if (FALSE == ret) {
		// TEMP : 에러 메시지 출력 (임시)
		// TODO : 에러가 난 소켓을 확인하고, 소켓을 닫는 로직 추가 <- SESSION을 닫아야 함
		int err_code = GetLastError();
		std::cout << "GetQueuedCompletionStatus Error: " << err_code << std::endl;
	}
}
