#pragma once
#include "DatabaseEvent.h"


class DatabaseManager
{
public:
	using DbOp = DatabaseEvent::DbOperation;

private:
	SQLHENV _henv{};
	SQLHDBC _hdbc{};
	// TODO: false sharing 막아야 함
	std::vector<SQLHSTMT> _hstmts;

	std::vector<std::thread> _threads;
	concurrency::concurrent_queue<DatabaseEvent> _queue;

public:
	DatabaseManager();
	DatabaseManager(uint64 handle_size);
	~DatabaseManager();

	void Init();

	void DatabaseThread();

	void AddEventDB(const DatabaseEvent& db_event);

private:
	void DatabaseWorker(int32 index_);

	void HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode);

};

