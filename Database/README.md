# Database setup

이 디렉터리에는 서버 개발 당시 사용했던 Microsoft SQL Server 데이터베이스에서 직접 추출한 스키마와 저장 프로시저가 들어 있습니다.

## 필요한 프로그램

- Microsoft SQL Server 2022 또는 SQL Server Express 2022
- SQL Server Management Studio(SSMS)
- Microsoft ODBC Driver 17 또는 18 for SQL Server(64비트)

원본 DB는 SQL Server 2022 Express에서 추출했습니다. SQL Server Express의 기본 인스턴스 이름을 사용했다면 일반적으로 서버 이름은 `.\SQLEXPRESS`입니다.

## 1. 데이터베이스 생성

1. SSMS를 실행합니다.
2. 서버 이름에 `.\SQLEXPRESS`를 입력하고 **Windows 인증**으로 연결합니다.
3. 개체 탐색기의 **데이터베이스**를 우클릭하고 **새 데이터베이스**를 선택합니다.
4. 데이터베이스 이름을 `MMORPGGameServer`로 지정해 생성합니다.

다른 이름을 사용해도 되지만, 이후 ODBC 설정에서 해당 데이터베이스를 기본 데이터베이스로 선택해야 합니다.

## 2. 테이블과 저장 프로시저 설치

1. SSMS에서 `original_schema.sql`을 엽니다.
2. 상단 데이터베이스 선택 목록에서 방금 만든 `MMORPGGameServer`를 선택합니다.
3. **실행**을 눌러 스크립트를 적용합니다.
4. 개체 탐색기를 새로 고쳐 `dbo.Player`, `dbo.PlayerInfo` 테이블과 저장 프로시저가 생성됐는지 확인합니다.

부하 테스트용 캐릭터 데이터도 필요하면 같은 데이터베이스를 선택한 상태에서 `stress_test_seed.sql`을 추가로 실행합니다. 일반적인 서버 실행에는 이 데이터가 필요하지 않습니다.

부하 테스트용 캐릭터 데이터도 복원하려면 스키마 설치 후 `stress_test_seed.sql`을 추가로 실행합니다. 이 파일에는 `P숫자` 형식의 가상 계정 10,000개만 포함되며, 실제 사용 이름으로 보이는 4개 레코드는 제외했습니다.

## 3. 64비트 ODBC System DSN 생성

1. Windows 시작 메뉴에서 **ODBC 데이터 원본(64비트)** 을 실행합니다.
2. **시스템 DSN** 탭에서 **추가**를 누릅니다.
3. `ODBC Driver 17 for SQL Server` 또는 `ODBC Driver 18 for SQL Server`를 선택합니다.
4. 데이터 원본 이름을 `GS2020180021`로 지정합니다.
5. 서버에는 `.\SQLEXPRESS`를 입력합니다.
6. Windows 통합 인증을 선택합니다.
7. 기본 데이터베이스를 `MMORPGGameServer`로 변경합니다.
8. Driver 18을 사용한다면 로컬 개발 환경에서는 서버 인증서 신뢰 옵션을 활성화해야 할 수 있습니다.
9. 마지막 단계에서 **데이터 원본 테스트**가 성공하는지 확인합니다.

반드시 **사용자 DSN**이 아닌 **시스템 DSN** 탭에 만들고, 32비트가 아닌 64비트 ODBC 관리자를 사용하세요.

## 4. 서버 설정

`Server.exe` 옆에 있는 `server.ini`의 DSN 이름을 위에서 만든 이름과 일치시킵니다.

```ini
[database]
dsn=GS2020180021
```

DSN을 다른 이름으로 만들었다면 `dsn=` 값만 변경하면 됩니다. 인증 비밀번호는 소스, SQL 스크립트 또는 `server.ini`에 포함되지 않으며 Windows 통합 인증을 사용합니다.

서버를 다른 PC의 클라이언트에서 접속할 경우 `server.ini`의 `listen_address=0.0.0.0`은 그대로 두고, Windows 방화벽에서 TCP 포트 `8252`의 인바운드 연결을 허용해야 합니다. 클라이언트 PC의 `client.ini`에는 서버 PC의 IP 주소를 입력합니다.

## 포함된 파일

- `original_schema.sql`: 원본 테이블, 기본 키와 저장 프로시저 6개
- `stress_test_seed.sql`: 공개 가능한 StressTest 계정 10,000개의 데이터
- `export_database.ps1`: 로컬 원본 DB에서 두 스크립트를 다시 생성하는 도구

원본에는 `dbo.Player`, `dbo.PlayerInfo` 테이블과 `find_id`, `insert_player`, `logout_process`, `match_id`, `register_request`, `save_player` 프로시저가 포함되어 있습니다. 현재 서버가 사용하는 프로시저는 `match_id`, `register_request`, `logout_process`입니다.

## 다시 추출하기

Windows PowerShell 5.1과 SQL Server PowerShell 모듈이 설치된 환경에서 다음과 같이 실행합니다.

```powershell
.\Database\export_database.ps1 -ServerInstance '.\SQLEXPRESS' -DatabaseName 'DB2020180021' -IncludeStressTestData
```

`-IncludeStressTestData`를 생략하면 스키마와 저장 프로시저만 추출합니다. 데이터 추출 조건은 `P` 뒤의 문자가 숫자로 변환되는 계정으로 제한되어 있으므로 일반 사용자 레코드는 출력하지 않습니다.

## Release에 포함하기

GitHub Release에는 실행 파일과 함께 다음과 같이 묶는 방식을 권장합니다.

```text
MMORPG-Game-Server-v1.0.0/
├─ Server.exe
├─ Client.exe
├─ StressTest.exe
├─ Resource/
├─ Database/
│  ├─ original_schema.sql
│  ├─ stress_test_seed.sql
│  └─ README.md
└─ README.md
```

`.bak` 파일과 달리 SQL 스크립트에는 SQL Server의 실제 데이터 파일 경로나 서버 버전별 백업 메타데이터가 포함되지 않습니다. 저장소와 Release에서 검토하기 쉽고, 일반 사용자 레코드도 제외할 수 있기 때문에 현재 프로젝트에는 이 방식이 더 적합합니다.
