/****** Object:  Table [dbo].[PlayerInfo]    Script Date: 2026-08-21(금) 오후 10:40:44 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[PlayerInfo](
	[user_id] [nchar](10) COLLATE Korean_Wansung_CI_AS NOT NULL,
	[pos_x] [int] NOT NULL,
	[pos_y] [int] NOT NULL
) ON [PRIMARY]
GO
/****** Object:  Table [dbo].[Player]    Script Date: 2026-08-21(금) 오후 10:40:44 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Player](
	[UserID] [bigint] IDENTITY(1,1) NOT NULL,
	[PlayerID] [nchar](20) COLLATE Korean_Wansung_CI_AS NOT NULL,
	[posX] [smallint] NOT NULL,
	[posY] [smallint] NOT NULL,
	[MaxHP] [int] NOT NULL,
	[HP] [int] NOT NULL,
	[ClassType] [tinyint] NOT NULL,
	[UserLevel] [int] NOT NULL,
	[UserExp] [bigint] NOT NULL,
 CONSTRAINT [PK_Player] PRIMARY KEY CLUSTERED 
(
	[UserID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
) ON [PRIMARY]
GO
/****** Object:  StoredProcedure [dbo].[save_player]    Script Date: 2026-08-21(금) 오후 10:40:44 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE save_player 
	@id NCHAR(10),
	@x INT,
	@y INT
AS
BEGIN
	SET NOCOUNT ON;

	UPDATE dbo.PlayerInfo
	SET pos_x = @x, pos_y = @y
	WHERE user_id = @id
END
GO
/****** Object:  StoredProcedure [dbo].[register_request]    Script Date: 2026-08-21(금) 오후 10:40:44 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[register_request] 
    @nickname     nchar(20),
    @pos_x        smallint,
    @pos_y        smallint,
    @maxHP        smallint,
    @HP           smallint,
    @class_type   tinyint,
    @level        int,
    @exp          bigint,
    @result_code  int OUTPUT, -- 0: success, 1: duplicate
    @user_id      bigint OUTPUT
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- 닉네임 중복 검사
    IF EXISTS (SELECT 1 FROM dbo.Player WHERE PlayerID = @nickname)
    BEGIN
        SET @result_code = 1; -- 실패 (중복)
        RETURN;
    END

    -- 신규 유저 등록
    INSERT INTO dbo.Player (PlayerID, PosX, PosY, MaxHP, HP, ClassType, UserLevel, UserExp)
    VALUES (@nickname, @pos_x, @pos_y, @maxHP, @HP, @class_type, @level, @exp);

    SET @user_id = SCOPE_IDENTITY();
    SET @result_code = 0; -- 성공
END
GO
/****** Object:  StoredProcedure [dbo].[match_id]    Script Date: 2026-08-21(금) 오후 10:40:44 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE match_id 
	@nickname nchar(20)
AS
BEGIN
	SET NOCOUNT ON;

	SELECT UserID, posX, posY, MaxHP, HP, ClassType, UserLevel, UserExp
	FROM dbo.Player
	WHERE PlayerID = @nickname;
END
GO
/****** Object:  StoredProcedure [dbo].[logout_process]    Script Date: 2026-08-21(금) 오후 10:40:44 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE logout_process 
	@userID		　Bigint,
	@x			　smallint,
	@y			　smallint,
    @maxHP        smallint,
    @HP           smallint,
    @level        int,
    @exp          bigint,
    @result_code  int OUTPUT -- 0: success, 1: false (Don't Exist ID)
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

	-- 존재 여부 확인
	IF NOT EXISTS (SELECT 1 FROM dbo.Player WHERE UserID = @userID)
	BEGIN
		SET @result_code = 1; -- 실패
		RETURN;
	END

    -- Insert statements for procedure here
	UPDATE dbo.Player
	SET 
		PosX = @x,
		PosY = @y,
		MaxHP = @maxHP,
		HP    = @HP,
		UserLevel = @level,
		UserExp   = @exp
	WHERE UserID = @userID;

	SET @result_code = 0;
END
GO
/****** Object:  StoredProcedure [dbo].[insert_player]    Script Date: 2026-08-21(금) 오후 10:40:44 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[insert_player] 
	@id NCHAR(10), 
	@x INT, 
	@y INT
AS
BEGIN
	SET NOCOUNT ON;

	INSERT INTO dbo.PlayerInfo (user_id, pos_x, pos_y)
	VALUES(@id, @x, @y)
END
GO
/****** Object:  StoredProcedure [dbo].[find_id]    Script Date: 2026-08-21(금) 오후 10:40:44 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE find_id
	@id NCHAR(10)
AS
BEGIN
	SET NOCOUNT ON;

	SELECT pos_x, pos_y
	FROM dbo.PlayerInfo
	WHERE user_id = @id;
END
GO
