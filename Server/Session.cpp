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
	// WSASend()
}

void Session::Recv()
{
	// TODO : Recv Overlapped 구조체를 만들어서 Recv 하기
	// WSARecv()
}

void Session::ProcessPacket(BYTE* packet)
{
	// TODO : 패킷 프로토콜에 맞게 분석 후 처리하기

}
