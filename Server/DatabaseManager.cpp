#include "pch.h"
#include "DatabaseManager.h"


DatabaseManager::DatabaseManager()
	: DatabaseManager{ 1 }
{

}

DatabaseManager::DatabaseManager(uint64 handle_size)
	: _hstmts{ handle_size }
{

}

void DatabaseManager::Init()
{
	SQLRETURN retcode;
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_henv);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
		HandleDiagnosticRecord(_henv, SQL_HANDLE_ENV, retcode);
		exit(-1);
	}

	retcode = SQLSetEnvAttr(_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
		HandleDiagnosticRecord(_henv, SQL_HANDLE_ENV, retcode);
		exit(-1);
	}

	retcode = SQLAllocHandle(SQL_HANDLE_DBC, _henv, &_hdbc);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
		HandleDiagnosticRecord(_hdbc, SQL_HANDLE_DBC, retcode);
		exit(-1);
	}
	SQLSetConnectAttr(_hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
	retcode = SQLConnect(_hdbc, (SQLWCHAR*)L"GS2020180021", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);
	if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
		HandleDiagnosticRecord(_hdbc, SQL_HANDLE_DBC, retcode);
		exit(-1);
	}

	std::cout << "DB Connected!!\n";

	for (auto& hstmt : _hstmts) {
		SQLAllocHandle(SQL_HANDLE_STMT, _hdbc, &hstmt);
	}
	std::cout << "DB SQL Handle " << _hstmts.size() << " created...\n";
}

void DatabaseManager::DatabaseThread()
{
	_threads.reserve(_hstmts.size());

	for (int i = 0; i < _hstmts.size(); ++i) {
		_threads.emplace_back([this]() { DatabaseWorker(); });
	}

	std::cout << "DB Thread Init Success\n";
	std::cout << "Thread Count : " << _threads.size() << "\n";
}

void DatabaseManager::DatabaseWorker()
{
	while (true) {
		DatabaseEvent db_event;
		if (_queue.try_pop(db_event)) {
			// TODO: DB OP에 따라 처리하기
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void DatabaseManager::HandleDiagnosticRecord(SQLHANDLE hHandle, SQLSMALLINT hType, RETCODE RetCode)
{
	SQLSMALLINT iRec = 0;
	SQLINTEGER  iError;
	WCHAR       wszMessage[1000];
	WCHAR       wszState[SQL_SQLSTATE_SIZE + 1];

	if (RetCode == SQL_INVALID_HANDLE) {
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}
	while (SQLGetDiagRec(hType, hHandle, ++iRec, wszState, &iError, wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)), (SQLSMALLINT*)NULL) == SQL_SUCCESS) {
		// Hide data truncated..
		if (wcsncmp(wszState, L"01004", 5)) {
			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}
}
