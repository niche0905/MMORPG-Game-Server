#pragma once
#include "Communication.h"
#include "BaseScene.h"


class BaseScene;
class Communication;


class Game
{
private:
	using Window = sf::RenderWindow;
	using Epoch = std::chrono::system_clock;
	using TimePoint = std::chrono::time_point<Epoch>;
	using MicroSeconds = std::chrono::microseconds;

private:
	Window window;
	sf::View view;

	std::shared_ptr<BaseScene> scene;

	TimePoint last_time;

	Communication communication;

	uint64 my_id;
	uint8 class_type;
	std::string name;

	bool is_developer = false;

public:
	Game();

	void Init();

	void Run();
	void Update(const int64 delta_time);
	void Draw();
	void HandleInput();

	void Exit();

	void SendID(const std::string& id);
	void SendRegister(uint8 class_type);
	void SendAttack(uint8 atk_key, uint8 atk_dir);
	void SendArrowKey(uint8 dir);
	void SendChatting(const std::string& str);
	void SendRespawn();

	void ProcessPacket(std::vector<BYTE> packet);

	bool HandleChatCommand(const std::string& command);

	void ConnectServer();

	void SetCameraView();

	void SceneLoad(SceneType scene_type);

	void SetID(uint64 id);
	uint64 GetID() const;

	void SetClassType(uint8 class_type);
	uint8 GetClassType() const;

	void SetName(const std::string& name);
	const std::string& GetName() const;

	bool IsDev() const;

};

