#include "Task.h"

#include "CameraControl.h"
#include"DebugTxt.h"
#include"EnemyControl.h"
#include"imgui.h"
#include"ChestControl.h"
#include "PlayerControl.h"
#include "TargetMarker.h"
#include"Field.h"
#include "GameOver.h"
#include "UI.h"

Task* Task::GetIns()
{
	static Task ins;
	return &ins;
}

void Task::Init()
{
	Sprite::LoadTexture(17, L"Resources/2d/PlayTask/getchest.png");
	Sprite::LoadTexture(18, L"Resources/2d/PlayTask/nogetchest.png");
	Sprite::LoadTexture(19, L"Resources/2d/LevelUp/debugfont - コピー.png");
	Sprite::LoadTexture(20, L"Resources/04 選択肢/button_select2_2.png");
	Sprite::LoadTexture(21, L"Resources/2d/PlayTask/Task1.png");
	Sprite::LoadTexture(22, L"Resources/2d/PlayTask/Task2.png");
	Sprite::LoadTexture(23, L"Resources/2d/PlayTask/Task3.png");
	Sprite::LoadTexture(24, L"Resources/2d/PlayTask/Task4.png");
	Sprite::LoadTexture(49, L"Resources/2d/Frame/Task4.png");
	Sprite::LoadTexture(26, L"Resources/2d/Frame/Task5.png");

	DebugTextSprite2::GetIns()->Initialize(19);

	TaskFrame.reset(Sprite::Create(20, {0, 0}));

	TasksSprite[TASK_ONE].reset(Sprite::Create(21, {0, 0}));
	TasksSprite[TASK_TWO].reset(Sprite::Create(22, {0, 0}));
	TasksSprite[TASK_THREE].reset(Sprite::Create(23, {0, 0}));
	TasksSprite[TASK_FOUR].reset(Sprite::Create(24, {0, 0}));
	TasksSprite[TASK_FIVE].reset(Sprite::Create(49, {0, 0}));

	//枠の位置大きさ

	{
		TaskFrame->SetAnchorPoint({0.5f, 0.5f});
		FramePos = {710.0f, 875.0f};
		FrameScl = {1700.0f, 113.0f};

		TaskPos = {960.0f - 200, 884.0f};
		TaskScl = {350.0f, 240.0f};

		for (int i = 0; i < TaskNum; i++)
		{
			TaskSpriteAlpha[i] = 0.0f;
			TasksSprite[i]->SetAnchorPoint({0.5, 0.5});
		}
		TaskSpriteAlpha[0] = 1.0f;
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
	TaskMenuPos = {1230 - 200.0f, 880.0f};

	Texture::LoadTexture(17, L"Resources/targetarrow.png");
	Texture::LoadTexture(18, L"Resources/2d/icon/nav1.png");
	Texture::LoadTexture(19, L"Resources/2d/icon/nav2.png");
	Texture::LoadTexture(20, L"Resources/2d/icon/nav3.png");
	Texture::LoadTexture(21, L"Resources/2d/icon/nav4.png");


	Texture* l_arrowTex = Texture::Create(17, {0.0f, -200.0f, 1}, {1.0f, 1.0f, 1.0f},
	                                      {1.0f, 1.0f, 1.0f, 1.0f});

	TargetArrow.reset(l_arrowTex);
	TargetArrow->CreateTexture();
	TargetArrow->SetAnchorPoint({0.5, 1.0});

	std::array<Texture*, 4> l_icon;
	l_icon[0] = Texture::Create(18, {0.0f, -200.0f, 1}, {1.0f, 1.0f, 1.0f},
	                            {1.0f, 1.0f, 1.0f, 1.0f});
	l_icon[1] = Texture::Create(19, {0.0f, -200.0f, 1}, {1.0f, 1.0f, 1.0f},
	                            {1.0f, 1.0f, 1.0f, 1.0f});
	l_icon[2] = Texture::Create(20, {0.0f, -200.0f, 1}, {1.0f, 1.0f, 1.0f},
	                            {1.0f, 1.0f, 1.0f, 1.0f});
	l_icon[3] = Texture::Create(21, {0.0f, -200.0f, 1}, {1.0f, 1.0f, 1.0f},
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
	Judg[TASK_TWO] = FlogDestCount > 1;
	Judg[TASK_THREE] = MiniGolemDestCount > 1;
	Judg[TASK_FOUR] = Field::GetIns()->GetPedestalPos().y < -50.0f;
	TaskFrame->SetPosition(FramePos);
	TaskFrame->SetSize(FrameScl);

	//[宝箱回収]スプライト
	for (int i = 0; i < 2; i++)
	{
		navChestSprite[i]->setcolor({1.f, 1.f, 1.f, navSpriteAlpha[i]});
		navChestSprite[i]->SetPosition(navTaskPos);
		navChestSprite[i]->SetSize(navTaskScl);
	}

	//[OOを倒しましょう]スプライト
	for (int i = 0; i < TaskNum; i++)
	{
		TasksSprite[i]->SetPosition(TaskPos);
		TasksSprite[i]->SetSize(TaskScl);
		TasksSprite[i]->setcolor({1.f, 1.f, 1.f, TaskSpriteAlpha[i]});
	}

	TasksSprite[1]->SetSize({TaskScl.x / 1.6f, TaskScl.y});

	//タスククリア後の次のタスへの遷移
	TaskClear(TASK_ONE, TASK_TWO, Judg[TASK_ONE], 2);
	TaskClear(TASK_TWO, TASK_THREE, Judg[TASK_TWO], 3);
	TaskClear(TASK_THREE, TASK_FOUR, Judg[TASK_THREE], 4);
	TaskClear(TASK_FOUR, TASK_FIVE, Judg[TASK_FOUR], 5);


	//宝箱が五個集まったら
	if (ChestControl::GetIns()->ChestCount() > 4)
	{
		TaskAllClear = true;
	}

	//タスクに進み具合用　int->string
	std::ostringstream str;
	if (tasks == TASK_ONE)
	{
		//タスク１->ゴーレム
		str << std::fixed << std::setprecision(2)
			<< GolemDestCount;
	}
	if (tasks == TASK_TWO)
	{
		//タスク２->トカゲ
		str << std::fixed << std::setprecision(2)
			<< FlogDestCount;
	}
	if (tasks == TASK_THREE)
	{
		//タスク３->みにゴーレム
		str << std::fixed << std::setprecision(2)
			<< MiniGolemDestCount;
	}
	//母数は２固定(後で変えるかも)
	if (target != PEDESTAL)
	{
		const std::string amount = "/2";
		DebugTextSprite2::GetIns()->Print(str.str() + amount, TaskMenuPos.x, TaskMenuPos.y, 0.8f);
	}
	//一番近い敵へのターゲット用
	//ゴーレム
	int nearIndex_Golem = TargetMarker::GetIns()->GetNearGolemIndex();
	//トカゲ
	int nearIndex_Lizard = TargetMarker::GetIns()->GetNearLizardIndex();
	//ミニゴーレム
	int nearIndex_MiniGolem = TargetMarker::GetIns()->GetNearMiniGolemIndex();

	Player* player = PlayerControl::GetIns()->GetPlayer();

	//探索エリアの敵配列から各エネミーごとに一番近いやつを抜き出す
	Enemy* targetenemy_Golem = EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[nearIndex_Golem].get();
	Enemy* targetenemy_MiniGolem = EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[nearIndex_MiniGolem].
		get();
	Enemy* targetenemy_Lizard = EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[nearIndex_Lizard].get();

	switch (target)
	{
	case GOLEM:
		TargetPos = targetenemy_Golem->GetPosition();
		arrowcol = {0.2f, 0.2f, 0.9f, 0.7f};
		break;

	case COW:
		TaskScl.y = 223.f;
		TargetPos = targetenemy_Lizard->GetPosition();
		arrowcol = {0.9f, 0.2f, 0.2f, 0.7f};
		break;
	case MINIGOLEM:
		TaskScl = {404.f, 205.f};
		TargetPos = targetenemy_MiniGolem->GetPosition();
		arrowcol = {0.7f, 0.7f, 0.7f, 0.7f};
		break;
	case CHEST:
		if (ChestControl::GetIns()->ChestCount() == 1)
		{
			TargetPos = ChestControl::GetIns()->GetChest(ChestControl::RED)->GetPosition();
		}
		if (ChestControl::GetIns()->ChestCount() == 2)
		{
			TargetPos = ChestControl::GetIns()->GetChest(ChestControl::BLUE)->GetPosition();
		}
		if (ChestControl::GetIns()->ChestCount() == 3)
		{
			TargetPos = ChestControl::GetIns()->GetChest(ChestControl::GREEN)->GetPosition();
		}
		arrowcol = {0.7f, 0.7f, 0.1f, 0.7f};
		break;
	case Bossarea:
		TargetPos = {17.f, -35.f, 820.f};
		break;

	case PEDESTAL:
		TargetPos = {-300.f, -30.f, 270.f};
	}

	if (Judg[TASK_ONE] == false)
	{
		target = GOLEM;
	}
	if (Judg[TASK_TWO] == false && Judg[TASK_ONE] == true)
	{
		target = COW;
	}
	if (Judg[TASK_THREE] == false && Judg[TASK_TWO] == true)
	{
		target = MINIGOLEM;
	}
	if (Judg[TASK_FOUR] == false && Judg[TASK_THREE] == true)
	{
		target = PEDESTAL;
		if (PlayerControl::GetIns()->GetPlayer()->GetHP() <= 0)
		{
			Judg[TASK_THREE] = false;
		}
	}
	if (Judg[TASK_ONE] == true && Judg[TASK_TWO] == true && Judg[TASK_THREE] == true && Judg[TASK_FOUR] == true)
	{
		target = Bossarea;
	}
	//敵がプエレイヤーの方向く処理
	XMVECTOR positionA = {
		player->GetPosition().x,
		player->GetPosition().y,
		player->GetPosition().z
	};
	XMVECTOR positionB = {TargetPos.x, TargetPos.y, TargetPos.z};
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(positionB, positionA); // positionA - positionB;

	float RotY;
	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
	//	if (enemy->GetFbxTime()< 510.000f / 60.000f) {
	curr = 55.f;
	TargetArrowRot = {
		90.f,
		0.f,
		RotY * -curr + 180.f,
	};

	TargetArrow->SetRotation(TargetArrowRot);
	TargetArrow->SetPosition({player->GetPosition().x, player->GetPosition().y + 0.0f, player->GetPosition().z});
	TargetArrow->SetScale({3.f, 4.f, 3.f});
	TargetArrow->SetBillboard(FALSE);
	TargetArrow->SetUVMove(TRUE);
	TargetArrow->SetColor(arrowcol);
	TargetArrow->Update(CameraControl::GetIns()->GetCamera());

	TaskSequence();
	for (int i = 0; i < 4; i++)
	{
		TargetIcon[i]->SetBillboard(TRUE);
		TargetIcon[i]->SetColor({1.f, 1.f, 1.f, iconalpha[i]});
		TargetIcon[i]->SetScale({3.f, 3.f, 3.f});
		TargetIcon[i]->SetPosition({player->GetPosition().x, player->GetPosition().y + 7.0f, player->GetPosition().z});

		TargetIcon[i]->Update(CameraControl::GetIns()->GetCamera());
	}
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
	if (tasks == TASK_THREE)
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
	TargetArrow->Draw();
	for (int i = 0; i < 4; i++)
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
		TaskFrame->Draw();
		for (int i = 0; i < TaskNum; i++)
		{
			TasksSprite[i]->Draw();
		}
		navChestSprite[0]->Draw();
		navChestSprite[1]->Draw();
		DebugTextSprite2::GetIns()->DrawAll();
		Sprite::PostDraw();
	}
	ImGui::Begin("task");
	ImGui::SliderFloat("tasksclx", &TaskScl.x, 0, 1000);
	ImGui::SliderFloat("taskscly", &TaskScl.y, 0, 1000);

	ImGui::End();
}

void Task::TaskSequence()
{
	Player* l_player = PlayerControl::GetIns()->GetPlayer();
	if (l_player == nullptr || l_player->GetStopFlag())
	{
		return;
	}

	Input* input = Input::GetIns();

	if (input->TiltPushStick(Input::L_UP, 0.0f) ||
		input->TiltPushStick(Input::L_DOWN, 0.0f) ||
		input->TiltPushStick(Input::L_RIGHT, 0.0f) ||
		input->TiltPushStick(Input::L_LEFT, 0.0f))
	{
		//	if (input->LeftTiltStick(input->Down) || input->LeftTiltStick(input->Up) || input->LeftTiltStick(input->Left) || input->LeftTiltStick(input->Right)) {
		movement++;
	}
	bool taskclear0 = movement > 60;
	bool taskclear1 = movement > 180;
	bool taskclear2 = movement > 300;
	bool taskclear3 = movement > 400;
	bool taskclear4 = ChestControl::GetIns()->ChestCount() == 2;
	if (taskclear0)
	{
		iconalpha[0] -= 0.02f;
	}
	if (iconalpha[0] <= 0.0f && !taskclear1)
	{
		iconalpha[1] += 0.02f;
	}
	if (taskclear1 && !taskclear2)
	{
		iconalpha[1] -= 0.02f;
	}
	if (taskclear2 && !taskclear3)
	{
		if (iconalpha[1] <= 0.0f)
		{
			iconalpha[2] += 0.02f;
			//	iconalpha[3] += 0.02f;
		}
	}
	if (taskclear3)
	{
		iconalpha[2] -= 0.02f;
	}
	if (taskclear3 && taskclear4)
	{
		if (iconalpha[2] <= 0.0f)
		{
			iconalpha[3] += 0.02f;
			//	iconalpha[3] += 0.02f;
		}
	}
	if (taskclear4)
	{
		if (movement > 460)
		{
			iconalpha[3] -= 0.02f;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		iconalpha[i] = min(iconalpha[i], 1.0f);
		iconalpha[i] = max(iconalpha[i], 0.0f);
	}
}

void Task::TaskClear(const Tasks& task, const Tasks& nexttask, bool clearjudg, int chestcount)
{
	if (tasks != task)
	{
		return;
	}
	if (clearjudg)
	{
		if (ChestControl::GetIns()->ChestCount() == chestcount)
		{
			TaskScl.x += 5.0f;
			TaskScl.y += 5.0f;
			navSpriteAlpha[0] -= 0.05f;
			navSpriteAlpha[1] += 0.05f;
			TaskSpriteAlpha[task] -= 0.05f;
			if (TaskSpriteAlpha[task] <= 0.0f)
			{
				TaskSpriteAlpha[nexttask] = 1.0f;
				tasks = nexttask;
			}
		}
		else
		{
			target = CHEST;
			navSpriteAlpha[1] -= 0.02f;
			navSpriteAlpha[0] += 0.02f;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		navSpriteAlpha[i] = min(navSpriteAlpha[i], 1.f);
		navSpriteAlpha[i] = max(navSpriteAlpha[i], 0.f);
	}
}
