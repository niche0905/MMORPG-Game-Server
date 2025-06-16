#pragma once


class ServerCore
{
private:
	template<typename K, typename V>
	using container = concurrency::concurrent_unordered_map<K, V>;
	template<typename T>
	using parallel_p_queue = concurrency::concurrent_priority_queue<T>;

	using Client = Creature*;

private:
	IocpCore					_iocp_core;			// IOCP 핸들 관리 하는 클래스
	container<uint64, Client>	_clients;			// 클라이언트 세션 관리하는 컨테이너
	container<uint64, uint64>	_login_user;		// 서버에 로그인 되어 있는 사람들을 Mapping (DB UserID -> Server SessionID)
	EBR<Session>				_ebr_sessions;		// ebr로 session 재사용

	DatabaseManager				_db_manager;		// DB 관리자
	SectorManager				_sector_manager;	// Sector 관리자

	volatile bool				_is_running;		// 서버가 실행중인지 체크하는 변수
	std::vector<std::thread>	_threads;			// 스레드 풀 관리하는 벡터

	parallel_p_queue<Event>		_timer_queue;		// Event 큐

	std::atomic<uint64>			_id_counter;		// 클라이언트 아이디 카운터
	SOCKET 						_listen_socket;		// 리슨 소켓 (서버 listen 용)
	SOCKET						_accept_socket;		// 클라이언트 소켓 (accept 전용)
	ExOver						_accept_overlapped;	// accept용 overlapped 구조체

	bool						_is_developer = false;

	WorldData					_world_map;			// WorldMap 정보

public:
	ServerCore();
	~ServerCore();

	void Init();
	void Run();
	void Release();

	void Accept();

private:
	void LocaleInit();

	void WorldInit();

	void NetworkInit();
	void BindAndListen();

	void DatabaseInit();

	void ThreadPoolInit();

	void StateInit();
	void NPCInit();

	void TesterInit();	// 한번만 쓸 거임

	void CreateListenSocket();
	void CreateAcceptSocket();

	void Worker();
	void TimerWorker();

public:
	const container<uint64, Client>& GetClients() const;
	container<uint64, Client>& GetClients();

	const container<uint64, uint64>& GetMappingTable() const;
	container<uint64, uint64>& GetMappingTable();


	void RegisterSector(uint64 id, int16 x, int16 y);
	void RegisterSector(uint64 id, Position pos);
	void MoveSector(uint64 id, int16 old_x, int16 old_y, int16 new_x, int16 new_y);
	void MoveSector(uint64 id, Position old_pos, Position new_pos);
	void RemoveSector(uint64 id, int16 x, int16 y);
	void RemoveSector(uint64 id, Position pos);

	std::unordered_set<uint64> GetClientList(short x, short y);
	std::unordered_set<uint64> GetClientList(Position pos);

	void AddTask(uint64 id, ExOver* new_task);
	void AddTimerEvent(const Event& timer_event);
	void AddRequestDB(const DatabaseEvent& db_event);

	bool IsBlock(int16 x, int16 y) const;
	bool IsBlock(const Position& pos) const;

	bool IsDev() const;

};

