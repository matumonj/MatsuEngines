#pragma once
#include"Sprite.h"
#include<memory>
#include"array"
class Task
{
public:
	static Task* GetInstance();

private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	static constexpr int TaskNum = 5;
	std::unique_ptr<Sprite>TaskFrame;
	std::array <std::unique_ptr<Sprite>,2>navChestSprite;
	std::array<float, 2>navSpriteAlpha;
	std::array<std::unique_ptr<Sprite>, TaskNum>TasksSprite;
	std::array<bool, TaskNum>Judg;
	std::array<float, TaskNum>TaskSpriteAlpha;
	enum Tasks {
		TASK_ONE,
		TASK_TWO,
		TASK_THREE,
		TASK_FOUR,
		TASK_FIVE,
		TASK_COMPLEATE
	}tasks=Tasks::TASK_ONE;
	bool TaskAllClear;
private:
	XMFLOAT2 TaskMenuPos = {};
	XMFLOAT2 FramePos;
	XMFLOAT2 FrameScl;

	XMFLOAT2 TaskPos;
	XMFLOAT2 TaskScl;

	XMFLOAT2 navTaskPos;
	XMFLOAT2 navTaskScl;
public:
	void Init();
	void Upda();
	void Draw();

	void SetGolemDestroyCount() { GolemDestCount++; }
	void SetFlogDestroyCount() { FlogDestCount++; }

	bool GetAllTaskClear() { return TaskAllClear; }
	bool GetGolemDesthCount(int count) { if (GolemDestCount > count) { return true; }return false; }
	bool GetFlogDesthCount(int count) { if (FlogDestCount > count) { return true; }return false; }
private:
	int GolemDestCount;
	int FlogDestCount;
	void TaskClear(const Tasks& task, const Tasks& nexttask, bool clearjudg,int chestcunt);
};

