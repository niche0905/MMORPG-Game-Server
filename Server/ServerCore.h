#pragma once


class ServerCore
{
private:
	template<typename K, typename V>
	using container = concurrency::concurrent_unordered_map<K, V>;

	using Client = std::atomic<std::shared_ptr<Session>>;
	using LocalClient = std::shared_ptr<Session>;

private:
	IocpCore					_iocp_core;			// IOCP 핸들 관리 하는 클래스
	container<int64, Client>	_clients;			// 클라이언트 세션 관리하는 컨테이너

	volatile bool				_is_running;		// 서버가 실행중인지 체크하는 변수
	std::vector<std::thread>	_threads;			// 스레드 풀 관리하는 벡터

	std::atomic<int64>			_id_counter;		// 클라이언트 아이디 카운터
	SOCKET 						_listen_socket;		// 리슨 소켓 (서버 listen 용)
	SOCKET						_accept_socket;		// 클라이언트 소켓 (accept 전용)
	ExOver						_accept_overlapped;	// accept용 overlapped 구조체

public:
	ServerCore();
	~ServerCore();

	void Init();
	void Run();
	void Release();

	void Accept();

private:
	void NetworkInit();
	void BindAndListen();

	void ThreadPoolInit();

	void CreateListenSocket();
	void CreateAcceptSocket();

	void Worker();

};

