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

DatabaseManager::~DatabaseManager()
{
	for (auto& hstmt : _hstmts)
	{
		if (hstmt)
		{
			SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
			hstmt = nullptr;
		}
	}

	if (_hdbc)
	{
		SQLDisconnect(_hdbc); // 연결 끊기
		SQLFreeHandle(SQL_HANDLE_DBC, _hdbc); // Connection handle 해제
		_hdbc = nullptr;
	}

	if (_henv)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, _henv);
		_henv = nullptr;
	}
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

				const std::wstring match_id_query = L"EXEC match_id ?";
				wchar_t wname[100];
				size_t convertedChars = 0;
				mbstowcs_s(&convertedChars, wname, session->GetName().c_str(), strlen(session->GetName().c_str()) + 1);

				retcode = SQLPrepare(_hstmts[index], (SQLWCHAR*)match_id_query.c_str(), SQL_NTS);
				if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
					HandleDiagnosticRecord(_hstmts[index], SQL_HANDLE_STMT, retcode);
					exit(-1);
				}

				SQLBindParameter(_hstmts[index], 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 20, 0, (SQLPOINTER)wname, 0, NULL);
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
					SQLFreeStmt(_hstmts[index], SQL_CLOSE);
					exit(-1);
				}

				retcode = SQLFetch(_hstmts[index]);
				if (retcode == SQL_SUCCESS) {
					bool succ = session->LoginInfo(static_cast<uint64>(userID), x, y, maxHP, HP, class_type, level, exp);

					if (succ) {
						ExOver* new_task = new ExOver{ OverOperation::DB_LOGIN };
						server.AddTask(now_id, new_task);
					}
				}
				else if (retcode == SQL_NO_DATA) {
					ExOver* new_task = new ExOver{ OverOperation::DB_LOGIN_FAIL };
					server.AddTask(now_id, new_task);
				}
				else {
					HandleDiagnosticRecord(_hstmts[index], SQL_HANDLE_STMT, retcode);
					SQLFreeStmt(_hstmts[index], SQL_CLOSE);
					exit(-1);
				}

				SQLCloseCursor(_hstmts[index]);

			}
			break;

			case DatabaseEvent::DB_REGISTER_REQUEST:
			{
				Creature* creature = server.GetClients()[now_id];
				if (creature == nullptr) break;
				Session* session = static_cast<Session*>(creature);

				const std::wstring match_id_query = L"EXEC register_request ? ? ? ? ? ? ? ? ?";
				wchar_t wname[100];
				size_t convertedChars = 0;
				mbstowcs_s(&convertedChars, wname, session->GetName().c_str(), strlen(session->GetName().c_str()) + 1);

				retcode = SQLPrepare(_hstmts[index], (SQLWCHAR*)match_id_query.c_str(), SQL_NTS);
				if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
					HandleDiagnosticRecord(_hstmts[index], SQL_HANDLE_STMT, retcode);
					SQLFreeStmt(_hstmts[index], SQL_CLOSE);
					exit(-1);
				}

				Position pos = session->GetPosition();
				int16 maxHP = 100, HP = 100;
				uint8 class_type = session->GetClassType();
				int32 level = 1;
				int64 exp = 0;
				int32 result_code = -1;

				SQLBindParameter(_hstmts[index], 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, 20, 0, (SQLPOINTER)wname, 0, NULL);
				SQLBindParameter(_hstmts[index], 2, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &pos.x, 0, NULL);
				SQLBindParameter(_hstmts[index], 3, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &pos.y, 0, NULL);
				SQLBindParameter(_hstmts[index], 4, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &maxHP, 0, NULL);
				SQLBindParameter(_hstmts[index], 5, SQL_PARAM_INPUT, SQL_C_SSHORT, SQL_SMALLINT, 0, 0, &HP, 0, NULL);
				SQLBindParameter(_hstmts[index], 6, SQL_PARAM_INPUT, SQL_C_UTINYINT, SQL_TINYINT, 0, 0, &class_type, 0, NULL);
				SQLBindParameter(_hstmts[index], 7, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &level, 0, NULL);
				SQLBindParameter(_hstmts[index], 8, SQL_PARAM_INPUT, SQL_C_SBIGINT, SQL_BIGINT, 0, 0, &exp, 0, NULL);
				SQLBindParameter(_hstmts[index], 9, SQL_PARAM_OUTPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &result_code, 0, NULL);

				retcode = SQLExecute(_hstmts[index]);
				if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
					HandleDiagnosticRecord(_hstmts[index], SQL_HANDLE_STMT, retcode);
					SQLFreeStmt(_hstmts[index], SQL_CLOSE);
					exit(-1);
				}

				if (result_code == 0) {
					// TODO: 등록 성공 반환 (login success 처리)
				}
				else {
					// TODO: 로그인 실패 처리
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
