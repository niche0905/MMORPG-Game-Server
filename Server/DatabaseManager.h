#pragma once


class DatabaseManager
{
private:
	SQLHENV _henv{};
	SQLHDBC _hdbc{};
	// TODO: false sharing 막아야 함
	std::vector<SQLHSTMT> _hstmts;

	std::vector<std::thread> _threads;
	// db event 클래스 만들어야함
	//concurrency::concurrent_queue<event_type> db_queue;

public:
	DatabaseManager();
	DatabaseManager(uint64 handle_size);

	void Init();

	void DatabaseThread();

private:
	void DatabaseWorker();

	void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);

};

