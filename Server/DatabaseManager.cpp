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
		_threads.emplace_back([this, i]() { DatabaseWorker(i); });
	}

	std::cout << "DB Thread Init Success\n";
	std::cout << "Thread Count : " << _threads.size() << "\n";
}

void DatabaseManager::AddEventDB(const DatabaseEvent& db_event)
{
	_queue.push(db_event);
}

void DatabaseManager::DatabaseWorker(int32 index_)
{
	int index = index_;
	SQLRETURN retcode;


	while (true) {
		DatabaseEvent db_event;
		if (_queue.try_pop(db_event)) {
			uint64 now_id = db_event._id;

			switch (db_event._sp_id)
			{
			case DatabaseEvent::DB_LOGIN_REQUEST:
			{
				Creature* creature = server.GetClients()[now_id];
				if (creature == nullptr) break;
				Session* session = static_cast<Session*>(creature);

				session->GetName();

				const std::wstring match_id_query = L"EXEC match_id ?";
				wchar_t wname[100];
				size_t convertedChars = 0;
				mbstowcs_s(&convertedChars, wname, session->GetName().c_str(), strlen(session->GetName().c_str()) + 1);

				retcode = SQLPrepare(_hstmts[index], (SQLWCHAR*)match_id_query.c_str(), SQL_NTS);
				if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
					HandleDiagnosticRecord(_hstmts[index], SQL_HANDLE_STMT, retcode);
					exit(-1);
				}

				SQLBindParameter(_hstmts[index], 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, wcslen(wname), 0, (SQLPOINTER)wname, 0, NULL);
				int64 userID;
				int16 x, y, maxHP, HP;
				uint8 class_type;
				int32 level;
				int64 exp; 
				SQLBindCol(_hstmts[index], 1, SQL_C_SBIGINT, &userID, 0, nullptr);
				SQLBindCol(_hstmts[index], 2, SQL_C_SSHORT, &x, 0, nullptr);
				SQLBindCol(_hstmts[index], 3, SQL_C_SSHORT, &y, 0, nullptr);
				SQLBindCol(_hstmts[index], 4, SQL_C_SSHORT, &maxHP, 0, nullptr);
				SQLBindCol(_hstmts[index], 5, SQL_C_SSHORT, &HP, 0, nullptr);
				SQLBindCol(_hstmts[index], 6, SQL_C_UTINYINT, &class_type, 0, nullptr);
				SQLBindCol(_hstmts[index], 7, SQL_C_SLONG, &level, 0, nullptr);
				SQLBindCol(_hstmts[index], 8, SQL_C_SBIGINT, &exp, 0, nullptr);

				retcode = SQLExecute(_hstmts[index]);
				if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
					HandleDiagnosticRecord(_hstmts[index], SQL_HANDLE_STMT, retcode);
					exit(-1);
				}

				retcode = SQLFetch(_hstmts[index]);
				if (retcode == SQL_SUCCESS) {
					session->LoginInfo(static_cast<uint64>(userID), x, y, maxHP, HP, class_type, level, exp);
					ExOver* new_task = new ExOver{ OverOperation::DB_LOGIN };
					server.AddTask(now_id, new_task);
				}
				else if (retcode == SQL_NO_DATA) {
					session->LoginFalse();
				}
				else {
					HandleDiagnosticRecord(_hstmts[index], SQL_HANDLE_STMT, retcode);
					exit(-1);
				}

				SQLCloseCursor(_hstmts[index]);

			}
			break;
			}
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
