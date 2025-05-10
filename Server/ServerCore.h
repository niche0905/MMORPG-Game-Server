#pragma once


class ServerCore
{
private:
	template<typename K, typename V>
	using container = concurrency::concurrent_unordered_map<K, V>;

	using Client = std::atomic<std::shared_ptr<Session>>;

private:
	IocpCore					_iocp_core;			// IOCP �ڵ� ���� �ϴ� Ŭ����
	container<int64, Client>	_clients;			// Ŭ���̾�Ʈ ���� �����ϴ� �����̳�

	volatile bool				_is_running;		// ������ ���������� üũ�ϴ� ����
	std::vector<std::thread>	_threads;			// ������ Ǯ �����ϴ� ����

	std::atomic<int64>			_id_counter;		// Ŭ���̾�Ʈ ���̵� ī����
	SOCKET 						_listen_socket;		// ���� ���� (���� listen ��)
	SOCKET						_accept_socket;		// Ŭ���̾�Ʈ ���� (accept ����)
	ExOver						_accept_overlapped;	// accept�� overlapped ����ü

public:
	ServerCore();
	~ServerCore();

	void Init();
	void Run();
	void Release();

	void Accept();
	void ProcessPacket(int64 id, BYTE* packet);

private:
	void NetworkInit();
	void BindAndListen();

	void ThreadPoolInit();

	void CreateListenSocket();
	void CreateAcceptSocket();

};

