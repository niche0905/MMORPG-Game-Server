#include "pch.h"
#include "Session.h"


Session::Session()
{
	// unordered_map�� ����� ���̱⿡ �⺻ �����ڰ� ȣ��Ǹ� �ȵǴ� ����
	std::cout << "�⺻ ������ ȣ�� ����\n";
}

Session::Session(SOCKET socket, int64 id) 
	: _socket(socket)
	, _id(id)
{

}

Session::~Session()
{


	// TODO : ������ �ݾƵ� �ȴٸ� �Ǵ� �Ŀ� �ݱ� or IOCP ���� �۾� �� ĵ���ϰ� �ٷ� �ݱ�
	closesocket(_socket);
}

void Session::Send(void* packet)
{
	// TODO : ��Ŷ �������ݿ� �°� �м� �� Packet Size�� �°� ����
	// WSASend()
}

void Session::Recv()
{
	// TODO : Recv Overlapped ����ü�� ���� Recv �ϱ�
	// WSARecv()
}

void Session::ProcessPacket(BYTE* packet)
{
	// TODO : ��Ŷ �������ݿ� �°� �м� �� ó���ϱ�

}
