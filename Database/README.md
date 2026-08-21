# Database setup

이 디렉터리에는 서버 개발 당시 사용했던 Microsoft SQL Server 데이터베이스에서 직접 추출한 스키마와 저장 프로시저가 들어 있습니다.

## 설치

1. SQL Server에 빈 데이터베이스를 하나 생성합니다.
2. SQL Server Management Studio에서 해당 데이터베이스를 선택합니다.
3. `original_schema.sql`을 실행합니다.
4. Windows의 **ODBC 데이터 원본(64비트)** 에서 SQL Server용 System DSN을 생성합니다.
5. 서버 코드의 `SQLConnect`에 지정된 DSN 이름과 동일하게 설정합니다.

부하 테스트용 캐릭터 데이터도 복원하려면 스키마 설치 후 `stress_test_seed.sql`을 추가로 실행합니다. 이 파일에는 `P숫자` 형식의 가상 계정 10,000개만 포함되며, 실제 사용 이름으로 보이는 4개 레코드는 제외했습니다.

현재 코드에 하드코딩된 DSN 이름은 `GS2020180021`입니다. 인증 정보는 소스나 SQL 스크립트에 포함되어 있지 않으며, DSN 설정에서 관리합니다.

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

GitHub Release에는 `setup.sql`을 단독으로 첨부하거나, 실행 파일과 함께 다음과 같이 묶는 방식을 권장합니다.

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
