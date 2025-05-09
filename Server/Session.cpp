#include "pch.h"
#include "Session.h"


Session::Session()
{
	// unordered_map을 사용할 것이기에 기본 생성자가 호출되면 안되는 것임
	std::cout << "기본 생성자 호출 에러\n";
}

Session::Session(SOCKET socket, int64 id) 
	: _socket(socket)
	, _id(id)
	, _recv_overlapped(IoOperation::IO_RECV)
	, _remain_size(0)
{

}

Session::~Session()
{


	// TODO : 소켓을 닫아도 된다면 판단 후에 닫기 or IOCP 소켓 작업 다 캔슬하고 바로 닫기
	closesocket(_socket);
}

void Session::Send(void* packet)
{
	// TODO : 패킷 프로토콜에 맞게 분석 후 Packet Size에 맞게 전송
	ExOver* send_overlapped = new ExOver(IoOperation::IO_SEND);
	// send_overlapped->SetBuffer(packet, packet[0/*패킷 사이즈가 저장된 위치*/]);

	DWORD sent_bytes = 0;
	auto ret = WSASend(_socket, send_overlapped->GetWSABuf(), 1, &sent_bytes, 0, send_overlapped->GetOverlapped(), nullptr);
	if (ret == SOCKET_ERROR) {
		// TEMP : 에러 메시지 출력 (임시)
		int error = WSAGetLastError();
		if (error != ERROR_IO_PENDING) {
			std::cout << "Send Error : " << error << "\n";
		}
	}
}

void Session::Recv()
{
	_recv_overlapped.Reset(_remain_size);

	DWORD flags = 0;
	auto ret = WSARecv(_socket, _recv_overlapped.GetWSABuf(), 1, nullptr, &flags, _recv_overlapped.GetOverlapped(), nullptr);
	if (ret == SOCKET_ERROR) {
		// TEMP : 에러 메시지 출력 (임시)
		int error = WSAGetLastError();
		if (error != ERROR_IO_PENDING) {
			std::cout << "Recv Error : " << error << "\n";
		}
	}
}

void Session::ProcessPacket(BYTE* packet)
{
	// TODO : 패킷 프로토콜에 맞게 분석 후 처리하기
	
}
