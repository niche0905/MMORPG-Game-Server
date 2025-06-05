#include "pch.h"
#include "Session.h"


Session::Session()
{
	// unordered_map을 사용할 것이기에 기본 생성자가 호출되면 안되는 것임
	std::cout << "Session 기본 생성자 호출 에러\n";
	exit(-1);
}

Session::Session(SOCKET socket, int64 id) 
	: Creature{ id, true }
	, _socket(socket)
	, _recv_overlapped(IoOperation::IO_RECV)
	, _remain_size(0)
{
	
}

Session::~Session()
{


	// TODO : 소켓을 닫아도 된다면 판단 후에 닫기 or IOCP 소켓 작업 다 캔슬하고 바로 닫기
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
	send_overlapped->SetBuffer(packet, static_cast<int>(reinterpret_cast<BYTE*>(packet)[0/*패킷 사이즈가 저장된 위치*/]));

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
	BASE_PACKET* packet_header = reinterpret_cast<BASE_PACKET*>(packet);
	uint8 packet_id = packet_header->_packet_id;

	switch (packet_id)
	{
	case PacketID::C2S_LOGIN:
	{
		LoginProcess(packet);
	}
	break;

	case PacketID::C2S_MOVE:
	{
		MoveProcess(packet);
	}
	break;

	case PacketID::C2S_CHAT:
	{
		ChatProcess(packet);
	}
	break;

	}
}

void Session::ReassemblePacket(DWORD recv_size)
{
	BYTE* packet = _recv_overlapped.GetBuffer();

	int data_size = _remain_size + recv_size;
	int proccess_size = 0;
	while (packet < _recv_overlapped.GetBuffer() + data_size) {
		BYTE packet_size = packet[0];

		if (data_size >= packet_size) {
			ProcessPacket(packet);
			packet += packet_size;
			proccess_size += packet_size;
		}
		else {
			break;
		}
	}

	if (data_size - proccess_size > 0) {
		_recv_overlapped.CopyToBuffer(packet, data_size - proccess_size);
	}
	_remain_size = data_size - proccess_size;
}

void Session::LoginProcess(BYTE* packet)
{
	CS_LOGIN_PACKET* login_packet = reinterpret_cast<CS_LOGIN_PACKET*>(packet);
	_nick_name = login_packet->_name;

	_position.x = rand() % MAX_WIDTH;
	_position.y = rand() % MAX_WIDTH;

	_state = State::ST_INGAME;

	// TODO: 값 제대로 사용하기
	SC_LOGIN_ALLOW_PACKET login_allow_packet{ _id, _position.x, _position.y, 100, 100, 1, 0 };
	Send(&login_allow_packet);
}

void Session::MoveProcess(BYTE* packet)
{
	// TODO: move_time 적용
	CS_MOVE_PACKET* move_packet = reinterpret_cast<CS_MOVE_PACKET*>(packet);

	Position new_position = _position;
	switch (move_packet->_direction)
	{
	case MOVE_UP:
		if (new_position.y > 0) --new_position.y;
		break;
	case MOVE_DOWN:
		if (new_position.y < MAX_HEIGHT - 1) ++new_position.y;
		break;
	case MOVE_LEFT:
		if (new_position.x > 0) --new_position.x;
		break;
	case MOVE_RIGHT:
		if (new_position.x < MAX_WIDTH) ++new_position.x;
		break;
	}

	if (_position == new_position) {
		// 변한게 없다면 스스로에게만 시간 Update
		SelfUpdate();
		return;
	}

	_position = new_position;
	SelfUpdate();

	// TODO: Creature가 ServerCore를 weak_ptr로 가지고 접근 할 수 있어야 함
	//		 그것을 가지고 Sector에 접근해 id와 clients로 broadcast 또는 호출
}

void Session::ChatProcess(BYTE* packet)
{
	// TODO: 내용 채우기 (일정 범위 내 전송)
}

void Session::SelfUpdate()
{
	SC_MOVE_PACKET update_packet{ _id, _position.x, _position.y, 0 };

	Send(&update_packet);
}
