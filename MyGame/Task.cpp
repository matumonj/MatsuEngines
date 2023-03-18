#include "Task.h"

#include <algorithm>

#include "CameraControl.h"
#include"DebugTxt.h"
#include"EnemyControl.h"
#include"imgui.h"
#include"ChestControl.h"
#include "PlayerControl.h"
#include "TargetMarker.h"
#include"Field.h"
#include "FootSwitch.h"
#include "GameOver.h"
#include "ImageManager.h"
#include "UI.h"

#include"mHelper.h"

Task* Task::GetIns()
{
	static Task ins;
	return &ins;
}

void Task::Init()
{
	DebugTextSprite2::GetIns()->Initialize(ImageManager::DEBUGFONT);
	for (auto i = 0; i < TaskNum; i++)
	{
		TaskFrame[i].reset(Sprite::Create(ImageManager::GetIns()->TASKFRAME, {0, 0}));
		ClearTaskLine[i].reset(Sprite::Create(ImageManager::CHESTANNOUNCE, {0, 0}));
	}
	TasksSprite[TASK_ONE].reset(Sprite::Create(ImageManager::TASK1, {0, 0}));
	TasksSprite[TASK_TWO].reset(Sprite::Create(ImageManager::TASK3, {0, 0}));
	TasksSprite[TASK_THREE].reset(Sprite::Create(ImageManager::TASK2, {0, 0}));
	TasksSprite[TASK_FOUR].reset(Sprite::Create(ImageManager::TASK4, {0, 0}));

	TaskMenuList[0].reset(Sprite::Create(ImageManager::OPENTASK, {0, 0}));
	TaskMenuList[1].reset(Sprite::Create(ImageManager::CLOSETASK, {0, 0}));

	FrameScl = {970.f, 100.f};
	//枠の位置大きさ

	{
		TaskScl = {350.0f, 240.0f};

		for (int i = 0; i < TaskNum; i++)
		{
			TaskFrame[i]->SetAnchorPoint({0.5f, 0.5f});
			FramePos = {710.0f, 875.0f};
			FrameScl = {900.0f, 113.0f};
			TasksSprite[i]->SetAnchorPoint({0.5, 0.5});

			ClearTaskLine[i]->SetAnchorPoint({0.5f, 0.5f});
		}
	}
	//
	{
		navTaskPos = {590 - 200.0f, 877.0f};
		navTaskScl = {300.0f, 200.0f};
		navChestSprite[0].reset(Sprite::Create(17, {0, 0}));
		navChestSprite[1].reset(Sprite::Create(18, {0, 0}));
		navChestSprite[0]->SetAnchorPoint({0.5f, 0.5f});
		navChestSprite[1]->SetAnchorPoint({0.5f, 0.5f});
	}
	for (int i = 0; i < 5; i++)
	{
		Judg[i] = false;
	}
	navSpriteAlpha[0] = 0.0f;
	navSpriteAlpha[1] = 1.0f;
	TaskMenuPos = {1820.f, 520.0f};

	std::array<Texture*, 4> l_icon;
	l_icon[0] = Texture::Create(ImageManager::NAV1, {0.0f, -200.0f, 1}, {1.0f, 1.0f, 1.0f},
	                            {1.0f, 1.0f, 1.0f, 1.0f});
	l_icon[1] = Texture::Create(ImageManager::NAV2, {0.0f, -200.0f, 1}, {1.0f, 1.0f, 1.0f},
	                            {1.0f, 1.0f, 1.0f, 1.0f});
	l_icon[2] = Texture::Create(ImageManager::NAV3, {0.0f, -200.0f, 1}, {1.0f, 1.0f, 1.0f},
	                            {1.0f, 1.0f, 1.0f, 1.0f});
	l_icon[3] = Texture::Create(ImageManager::NAV4, {0.0f, -200.0f, 1}, {1.0f, 1.0f, 1.0f},
	                            {1.0f, 1.0f, 1.0f, 1.0f});


	for (int i = 0; i < TargetIcon.size(); i++)
	{
		TargetIcon[i].reset(l_icon[i]);
		TargetIcon[i]->CreateTexture();
		TargetIcon[i]->SetAnchorPoint({0.5f, 1.0f});
		TargetIcon[i]->SetRotation({180, 0, 0});
		iconalpha[i] = 0.0f;
	}
	tasks = TASK_ONE;
	iconalpha[0] = 1.0f;
}

void Task::Upda()
{
	//宝箱出現位置
	Judg[TASK_ONE] = GolemDestCount > 1;
	Judg[TASK_TWO] = MiniGolemDestCount > 1;
	Judg[TASK_THREE] = FootSwitch::GetIns()->GetClearSwicthsQuantity() > 3;
	Judg[TASK_FOUR] = GuardianDeathCount > 0;

	for (auto i = 0; i < TaskNum; i++)
	{
		if (Judg[i])
		{
			ClearTaskLineEaseC[i] += 0.04f;

			ClearTaskLineScl[i].y = TaskScl.y;
			ClearTaskLineScl[i].x = Easing::EaseOut(ClearTaskLineEaseC[i], 0.f, TaskScl.x);
			ClearTaskLineEaseC[i] = std::clamp(ClearTaskLineEaseC[i], 0.f, 1.f);
		}
	}
	//[宝箱回収]スプライト
	for (int i = 0; i < 2; i++)
	{
		//タスク一覧表示するかどうかテクスチャのパラメータセット
		TaskMenuList[i]->SetPosition({1550.0f, 220.f});
		TaskMenuList[i]->SetSize({350.f, 203.f});
		TaskMenuList[i]->setcolor({1.f, 1.f, 1.f, TaskMenuListAlpha[i]});

		//宝箱回収数のテクスチャパラメータセット
		navChestSprite[i]->setcolor({1.f, 1.f, 1.f, navSpriteAlpha[i]});
		navChestSprite[i]->SetPosition(navTaskPos);
		navChestSprite[i]->SetSize(navTaskScl);
	}

	//[OOを倒しましょう]スプライト
	for (int i = 0; i < TaskNum; i++)
	{
		//タスク座標Y座標はメニュー開いたとき間隔開ける
		TaskPos[i] = {1550.0f, 520.f + static_cast<float>(i) * SpritePosInter};

		//タスクフレームのパラメータセット
		TaskFrame[i]->SetPosition({TaskPos[i].x + 100.f, TaskPos[i].y});
		TaskFrame[i]->SetSize(FrameScl);
		TaskFrame[i]->setcolor({1.f, 1.f, 1.f, TaskSpriteAlpha[i]});

		//タスクのパラメータセット
		TasksSprite[i]->SetPosition(TaskPos[i]);
		TasksSprite[i]->SetSize(TaskScl);
		TasksSprite[i]->setcolor({1.f, 1.f, 1.f, TaskSpriteAlpha[i]});

		ClearTaskLine[i]->SetPosition(TaskPos[i]);
		ClearTaskLine[i]->SetSize(ClearTaskLineScl[i]);
		ClearTaskLine[i]->setcolor({1.f, 1.f, 1.f, TaskSpriteAlpha[i]});
	}

	//宝箱が五個集まったら
	TaskAllClear = ChestControl::GetIns()->ChestCount() > 4;

	//タスクに進み具合用　int->string
	std::array<std::ostringstream, TaskNum> taskstr;
	taskstr[TASK_ONE] << std::fixed << std::setprecision(0)
		<< GolemDestCount;
	taskstr[TASK_TWO] << std::fixed << std::setprecision(0)
		<< MiniGolemDestCount;
	taskstr[TASK_THREE] << std::fixed << std::setprecision(2)
		<< FootSwitch::GetIns()->GetClearSwicthsQuantity();

	taskstr[TASK_FOUR] << std::fixed << std::setprecision(0)
		<< GuardianDeathCount; // EnemyControl::GetIns()->GetGuardianEnemy()->getdeath();

	//後で関数化スべき
	const std::string amount_Golem = "/2";
	DebugTextSprite2::GetIns()->Print(taskstr[TASK_ONE].str() + amount_Golem, TaskMenuPos.x - 20.f, TaskMenuPos.y,
	                                  taskNumSize);
	const std::string amount_MiniGolem = "/2";
	DebugTextSprite2::GetIns()->Print(taskstr[TASK_TWO].str() + amount_MiniGolem, TaskMenuPos.x - 20.f,
	                                  TaskMenuPos.y + TaskNumInterPos, taskNumSize);
	const std::string amount_Switch = "/4";
	DebugTextSprite2::GetIns()->Print(taskstr[TASK_THREE].str() + amount_Switch, TaskMenuPos.x - 20.f,
	                                  TaskMenuPos.y + TaskNumInterPos * 2, taskNumSize);
	const std::string amount_Guardian = "/1";
	DebugTextSprite2::GetIns()->Print(taskstr[TASK_FOUR].str() + amount_Guardian, TaskMenuPos.x - 20.f,
	                                  TaskMenuPos.y + TaskNumInterPos * 3, taskNumSize);
	DebugTextSprite2::GetIns()->SetColor({1.f, 1.f, 1.f, TaskSpriteAlpha[0]});
	//一番近い敵へのターゲット用
	//ゴーレム
	int nearIndex_Golem = TargetMarker::GetIns()->GetNearGolemIndex();
	//トカゲ
	int nearIndex_Lizard = TargetMarker::GetIns()->GetNearLizardIndex();
	//ミニゴーレム
	int nearIndex_MiniGolem = TargetMarker::GetIns()->GetNearMiniGolemIndex();

	Player* player = PlayerControl::GetIns()->GetPlayer();

	OpenTasks();

	NavTaskSequence();
}

bool Task::ClearTaskONE()
{
	if (tasks == TASK_TWO)
	{
		return true;
	}
	return false;
}

bool Task::ClearTaskTwo()
{
	if (Judg[TASK_TWO])
	{
		return true;
	}
	return false;
}

bool Task::ClearTaskThree()
{
	if (tasks == TASK_FOUR)
	{
		return true;
	}
	return false;
}

void Task::TargetDraw()
{
	Texture::PreDraw();
	for (int i = 0; i < TargetIcon.size(); i++)
	{
		TargetIcon[i]->Draw();
	}
	Texture::PostDraw();
}

void Task::Draw()
{
	if (HUD::GetIns()->GetPlayerHP()->GetSize().x > 0.f && PlayerControl::GetIns()->GetPlayer()->GetHP() > 0)
	{
		Sprite::PreDraw();

		for (int i = 0; i < TaskNum; i++)
		{
			TaskFrame[i]->Draw();
			TasksSprite[i]->Draw();
			ClearTaskLine[i]->Draw();
		}
		navChestSprite[0]->Draw();
		navChestSprite[1]->Draw();

		TaskMenuList[0]->Draw();
		TaskMenuList[1]->Draw();
		DebugTextSprite2::GetIns()->DrawAll();
		Sprite::PostDraw();
	}
	ImGui::Begin("task");
	ImGui::SliderFloat("tasksclxa", &SpritePosInter, 0, 120); //95
	ImGui::SliderFloat("tasksyclya", &TaskMenuPos.x, 0, 1900);
	ImGui::SliderFloat("tasksclya", &TaskMenuPos.y, 0, 1000);
	ImGui::End();
}

void Task::NavTaskSequence()
{
	Player* l_player = PlayerControl::GetIns()->GetPlayer();
	Input* input = Input::GetIns();
	DebugCamera* camera = CameraControl::GetIns()->GetCamera();

	if (l_player == nullptr || l_player->GetStopFlag())
	{
		return;
	}

	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f))
	{
		movement++;
	}

	//アルファ値の増減分
	constexpr float AlphaVal = 0.02f;
	//冒頭のタスク進行説明テクスチャの切り替わり条件
	const bool taskclear[4] = {movement > 60, movement > 180, movement > 300, movement > 400};

	float prevAlpha = iconalpha[0];

	if (taskclear[0])
	{
		iconalpha[0] -= AlphaVal;
	}

	//一つ前のテクスチャアルファ値が0.f下回ったらつ語のテクスチャのアルファ値上げる
	for (int i = 1; i < 4; i++)
	{
		if (prevAlpha <= 0.f && !taskclear[i])
		{
			iconalpha[i] += AlphaVal;
			//代入してる値置き換え
			prevAlpha = iconalpha[i];
		}
		if (taskclear[i] && !taskclear[i + 1])
		{
			iconalpha[i] -= AlphaVal;
		}
		iconalpha[i] = std::clamp(iconalpha[i], 0.f, 1.0f);
	}

	for (auto i = 0; i < TargetIcon.size(); i++)
	{
		//冒頭のタスク進行説明テクスチャパラメータセット
		TargetIcon[i]->SetBillboard(TRUE);
		TargetIcon[i]->SetColor({1.f, 1.f, 1.f, iconalpha[i]});
		TargetIcon[i]->SetScale({3.f, 3.f, 3.f});
		TargetIcon[i]->SetPosition({
			l_player->GetPosition().x, l_player->GetPosition().y + 7.0f, l_player->GetPosition().z
		});
		TargetIcon[i]->Update(camera);
	}
}

void Task::OpenTasks()
{
	//イージングカウンタ加算値
	constexpr float EaseAddTime = 0.04f;

	//開く間隔の最低値と最高値
	constexpr float Min_FrameInter = 0.f;
	constexpr float Max_FrameInter = 110.f;

	//メニュー開閉条件
	bool l_CloseJudg = MovingFrameECount >= 1.f && Input::GetIns()->TriggerButton(Input::Y);
	bool l_OpenJudg = MovingFrameECount <= 0.f && Input::GetIns()->TriggerButton(Input::Y);

	float l_MaxEaseCount = 1.f;
	if (l_OpenJudg)
	{
		OpenJudg = true;
	}

	if (OpenJudg)
	{
		//イージング進める
		MovingFrameECount += EaseAddTime;
	}
	else
	{
		//イージング進める
		MovingFrameECount -= EaseAddTime;
	}
	if (l_CloseJudg)
	{
		OpenJudg = false;
	}

	//タスクテクスチャの間隔広げる
	SpritePosInter = Easing::EaseOut(MovingFrameECount, Min_FrameInter, Max_FrameInter);
	TaskMenuListAlpha[0] = Easing::EaseOut(MovingFrameECount, 1.f, 0.f);
	TaskMenuListAlpha[1] = Easing::EaseOut(MovingFrameECount, 0.f, 1.f);
	for (auto i = 0; i < TaskNum; i++)
	{
		l_MaxEaseCount = 1.f;
		if (Judg[i])
		{
			l_MaxEaseCount = 0.3f;
		}
		TaskSpriteAlpha[i] = Easing::EaseOut(MovingFrameECount, 0.f, l_MaxEaseCount);
	}
	//イージングカウンタの上限
	MovingFrameECount = std::clamp(MovingFrameECount, 0.f, 1.f);
}
