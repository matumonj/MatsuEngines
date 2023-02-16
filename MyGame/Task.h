#pragma once
#include"Sprite.h"
#include<memory>
#include"array"
#include "Texture.h"

#include"Object3d.h"

class Task
{
public:
	static Task* GetIns();

private:
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	static constexpr auto TaskNum = 4;
	static constexpr auto ChestNavNum = 2;
	static constexpr auto TaskListNavNum = 2;
	//タスク
	std::array<std::unique_ptr<Sprite>,TaskNum> TaskFrame;
	std::array<std::unique_ptr<Sprite>, TaskNum> TasksSprite;
	//クリアしたタスクの打ち消し線
	std::array<std::unique_ptr<Sprite>, TaskNum> ClearTaskLine;
	std::array<XMFLOAT2, TaskNum> ClearTaskLineScl;
	std::array<float, TaskNum> ClearTaskLineEaseC;

	//宝箱の回収率用のフレームとうち側てくちゃ
	std::array<std::unique_ptr<Sprite>,ChestNavNum> navChestSprite;
	std::array<float, ChestNavNum> navSpriteAlpha;
	//Yでタスク一覧を表示するかどうかのナビテクスチャ
	std::array<std::unique_ptr<Sprite>, TaskListNavNum> TaskMenuList;
	std::array<float, TaskListNavNum>TaskMenuListAlpha;
	//クリアしたかどうか
	std::array<bool, TaskNum> Judg;
	//テクスチャアルファ値
	std::array<float, TaskNum> TaskSpriteAlpha;
	std::array<std::unique_ptr<Texture>, TaskNum> TargetIcon;
	std::array<float, TaskNum> iconalpha;
	enum Tasks
	{
		TASK_ONE,
		TASK_TWO,
		TASK_THREE,
		TASK_FOUR,
		TASK_FIVE,
		TASK_COMPLEATE
	} tasks = TASK_ONE;

	//全てのタスククリアしたか
	bool TaskAllClear;
private:
	XMFLOAT2 TaskMenuPos = {};
	XMFLOAT2 FramePos;

	XMFLOAT2 FrameScl;

	std::array<XMFLOAT2,TaskNum> TaskPos;
	XMFLOAT2 TaskScl;

	float SpritePosInter;
	XMFLOAT2 navTaskPos;
	XMFLOAT2 navTaskScl;

private:
	int movement;
public:
	//初期化
	void Init();
	//更新
	void Upda();
	//描画
	void Draw();
	//タスク対象表示
	void TargetDraw();

	bool ClearTaskONE();
	bool ClearTaskTwo();
	bool ClearTaskThree();
	bool GetAllTaskClear() { return TaskAllClear; }

	inline bool GetGolemDesthCount(int count)
	{
		if (GolemDestCount == count) { return true; }
		return false;
	}

	inline bool GetMiniGolemDesthCount(int count)
	{
		if (MiniGolemDestCount == count) { return true; }
		return false;
	}

	void SetGuardianDeathCount(int count) { GuardianDeathCount = count; }
	bool TaskFourClear(Tasks task) { return Judg[task]; }
	
	void GuardianRset();
private:
	//ゴーレム倒した数
	int GolemDestCount;
	//ミニゴーレム足した数
	int MiniGolemDestCount;

	int GuardianDeathCount;
	void TaskClear(const Tasks& task, const Tasks& nexttask, bool clearjudg, int chestcunt);

	void NavTaskSequence();

	static constexpr float TaskNumInterPos=120.f;
	static constexpr float taskNumSize=0.5f;
public:
	void SetGolemDestroyCount() { GolemDestCount++; }
	void SetMiniGolemDestroyCount() { MiniGolemDestCount++; }

private:
	//タスク一覧表示するかどうか
	bool OpenJudg;
	//イージング用カウンタ
	float MovingFrameECount;

	void OpenTasks();
};
