#include "pch.h"
#include "Session.h"


Session::Session()
{
	// unordered_map�� ����� ���̱⿡ �⺻ �����ڰ� ȣ��Ǹ� �ȵǴ� ����
	std::cout << "Session �⺻ ������ ȣ�� ����\n";
	exit(-1);
}

Session::Session(SOCKET socket, int64 id) 
	: BaseSession{ id }
	, _socket(socket)
	, _recv_overlapped(IoOperation::IO_RECV)
	, _remain_size(0)
{
	
}

Session::~Session()
{


	// TODO : ������ �ݾƵ� �ȴٸ� �Ǵ� �Ŀ� �ݱ� or IOCP ���� �۾� �� ĵ���ϰ� �ٷ� �ݱ�
	closesocket(_socket);
}

bool Session::IsPlayer() const
{
	return true;
}

bool Session::IsNPC() const
{
	return false;
}

void Session::Send(void* packet)
{
	ExOver* send_overlapped = new ExOver(IoOperation::IO_SEND);
	send_overlapped->SetBuffer(packet, static_cast<int>(reinterpret_cast<BYTE*>(packet)[0/*��Ŷ ����� ����� ��ġ*/]));

	DWORD sent_bytes = 0;
	auto ret = WSASend(_socket, send_overlapped->GetWSABuf(), 1, &sent_bytes, 0, send_overlapped->GetOverlapped(), nullptr);
	if (ret == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error != ERROR_IO_PENDING) {
			PrintErrorMessage(L"WSASend", error);
		}
	}
}

void Session::Recv()
{
	_recv_overlapped.Reset(_remain_size);

	DWORD flags = 0;
	auto ret = WSARecv(_socket, _recv_overlapped.GetWSABuf(), 1, nullptr, &flags, _recv_overlapped.GetOverlapped(), nullptr);
	if (ret == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error != ERROR_IO_PENDING) {
			PrintErrorMessage(L"WSARecv", error);
		}
	}
}

void Session::ProcessPacket(BYTE* packet)
{
	// TODO : ��Ŷ �������ݿ� �°� �м� �� ó���ϱ�
	
}

void Session::ReassemblePacket(DWORD recv_size)
{
	BYTE* packet = _recv_overlapped.GetBuffer();
	int data_size = _remain_size + recv_size;
	while (true) {
		BYTE packet_size = packet[0];

		if (data_size >= packet_size) {
			ProcessPacket(packet);
			packet += packet_size;
			data_size -= packet_size;
		}
		else {
			break;
		}
	}

	if (data_size > 0) {
		_recv_overlapped.CopyToBuffer(packet, data_size);
	}
	_remain_size = data_size;
}
