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
	std::array<std::unique_ptr<Sprite>, TaskNum>TasksSprite;
	std::array<bool, TaskNum>Judg;
	enum Tasks {
		TASK_ONE,
		TASK_TWO,
		TASK_THREE,
		TASK_FOUR,
		TASK_FIVE,
		TASK_COMPLEATE
	}tasks=Tasks::TASK_ONE;

private:
	XMFLOAT2 TaskMenuPos = {};
	XMFLOAT2 FramePos;
	XMFLOAT2 FrameScl;
public:
	void Init();
	void Upda();
	void Draw();

	void SetGolemDestroyCount() { GolemDestCount++; }
	void SetFlogDestroyCount() { FlogDestCount++; }
private:
	int GolemDestCount;
	int FlogDestCount;
	void TaskClear(const Tasks& task, bool clearjudg);
};

