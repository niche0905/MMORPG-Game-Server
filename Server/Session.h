#pragma once


class Session : public BaseSession
{
private:
	SOCKET		_socket;

	ExOver		_recv_overlapped;
	int32		_remain_size;

	// ���ӿ� �ʿ��� ���� (������ ����)

public:
	Session();
	Session(SOCKET socket, int64 id);

	~Session();

	bool IsPlayer() const override;
	bool IsNPC() const override;

	void Send(void* packet);
	void Recv();

	void ProcessPacket(BYTE* packet);	// ��� private �̿��� �� ��?
	void ReassemblePacket(DWORD recv_size);

};

