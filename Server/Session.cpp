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
	, _recv_overlapped(IoOperation::IO_RECV)
	, _remain_size(0)
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
	ExOver* send_overlapped = new ExOver(IoOperation::IO_SEND);
	// send_overlapped->SetBuffer(packet, packet[0/*��Ŷ ����� ����� ��ġ*/]);

	DWORD sent_bytes = 0;
	auto ret = WSASend(_socket, send_overlapped->GetWSABuf(), 1, &sent_bytes, 0, send_overlapped->GetOverlapped(), nullptr);
	if (ret == SOCKET_ERROR) {
		// TEMP : ���� �޽��� ��� (�ӽ�)
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
		// TEMP : ���� �޽��� ��� (�ӽ�)
		int error = WSAGetLastError();
		if (error != ERROR_IO_PENDING) {
			std::cout << "Recv Error : " << error << "\n";
		}
	}
}

void Session::ProcessPacket(BYTE* packet)
{
	// TODO : ��Ŷ �������ݿ� �°� �м� �� ó���ϱ�
	
}
