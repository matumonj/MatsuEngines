#include "Task.h"

#include "CameraControl.h"
#include"DebugTxt.h"
#include"EnemyControl.h"
#include"imgui.h"
#include"ChestControl.h"
#include "PlayerControl.h"
#include "TargetMarker.h"

Task* Task::GetInstance()
{
	static Task ins;
	return&ins;
}

void Task::Init()
{
	Sprite::LoadTexture(17, L"Resources/2d/PlayTask/getchest.png");
	Sprite::LoadTexture(18, L"Resources/2d/PlayTask/nogetchest.png");
	Sprite::LoadTexture(19, L"Resources/2d/LevelUp/debugfont - コピー.png");
	Sprite::LoadTexture(20, L"Resources/04 選択肢/button_select2_2.png");
	Sprite::LoadTexture(21, L"Resources/2d/PlayTask/Task1.png");
	Sprite::LoadTexture(22, L"Resources/2d/PlayTask/Task2.png");
	Sprite::LoadTexture(23, L"Resources/2d/Frame/Task3.png");
	Sprite::LoadTexture(24, L"Resources/2d/Frame/Task4.png");
	Sprite::LoadTexture(25, L"Resources/2d/Frame/Task5.png");

	DebugTextSprite2::GetInstance()->Initialize(19);

	TaskFrame.reset(Sprite::Create(20, { 0,0 }));

	TasksSprite[Tasks::TASK_ONE].reset(Sprite::Create(21, { 0,0 }));
	TasksSprite[Tasks::TASK_TWO].reset(Sprite::Create(22, { 0,0 }));
	TasksSprite[Tasks::TASK_THREE].reset(Sprite::Create(23, { 0,0 }));
	TasksSprite[Tasks::TASK_FOUR].reset(Sprite::Create(24, { 0,0 }));
	TasksSprite[Tasks::TASK_FIVE].reset(Sprite::Create(25, { 0,0 }));

	//枠の位置大きさ

	{
		TaskFrame->SetAnchorPoint({ 0.5f,0.5f });
		FramePos = { 710.0f,875.0f };
		FrameScl = { 1700.0f,113.0f };

		TaskPos = { 960.0f - 200,884.0f };
		TaskScl = { 350.0f,240.0f };

		for (int i = 0; i < TaskNum; i++)
		{
			TaskSpriteAlpha[i] = 0.0f;
			TasksSprite[i]->SetAnchorPoint({ 0.5,0.5 });
		}
		TaskSpriteAlpha[0] = 1.0f;
	}
	//
	{
		navTaskPos = { 590 - 200.0f,877.0f };
		navTaskScl = { 300.0f,200.0f };
		navChestSprite[0].reset(Sprite::Create(17, {0,0}));
		navChestSprite[1].reset(Sprite::Create(18, {0,0}));
		navChestSprite[0]->SetAnchorPoint({0.5f,0.5f});
		navChestSprite[1]->SetAnchorPoint({ 0.5f,0.5f });
	}
	navSpriteAlpha[0] = 0.0f;
	navSpriteAlpha[1] = 1.0f;
	TaskMenuPos = { 1230-200.0f,880.0f };

	Texture::LoadTexture(17, L"Resources/targetarrow.png");
	Texture::LoadTexture(18, L"Resources/2d/icon/nav1.png");
	Texture::LoadTexture(19, L"Resources/2d/icon/nav2.png");
	Texture::LoadTexture(20, L"Resources/2d/icon/nav3.png");
	Texture::LoadTexture(21, L"Resources/2d/icon/nav4.png");


	Texture* l_arrowTex  = Texture::Create(17, { 0.0f, -200.0f, 1 }, { 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f });
		
	TargetArrow.reset(l_arrowTex);
	TargetArrow->CreateTexture();
	TargetArrow->SetAnchorPoint({ 0.5,1.0 });

	std::array<Texture*,4>l_icon;
	l_icon[0] = Texture::Create(18, {0.0f, -200.0f, 1}, {1.0f, 1.0f, 1.0f},
		{ 1.0f, 1.0f, 1.0f, 1.0f });
	l_icon[1] = Texture::Create(19, { 0.0f, -200.0f, 1 }, { 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f });
	l_icon[2] = Texture::Create(20, { 0.0f, -200.0f, 1 }, { 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f });
	l_icon[3] = Texture::Create(21, { 0.0f, -200.0f, 1 }, { 1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f });
	

	for (int i = 0; i < TargetIcon.size(); i++)
	{
		TargetIcon[i].reset(l_icon[i]);
		TargetIcon[i]->CreateTexture();
		TargetIcon[i]->SetAnchorPoint({ 0.5f,1.0f });
		TargetIcon[i]->SetRotation({ 180,0,0 });
	}
	iconalpha[0] = 1.0f;
}

void Task::Upda()
{
	//宝箱出現位置
	Judg[Tasks::TASK_ONE] = GolemDestCount > 1;
	Judg[Tasks::TASK_TWO] = FlogDestCount > 1;

	TaskFrame->SetPosition(FramePos);
	TaskFrame->SetSize(FrameScl);

	for (int i = 0; i < 2; i++) {
		navChestSprite[i]->setcolor({ 1.f,1.f,1.f,navSpriteAlpha[i] });
		navChestSprite[i]->SetPosition(navTaskPos);
		navChestSprite[i]->SetSize(navTaskScl);
	}

	for (int i = 0; i<TaskNum; i++)
	{
		TasksSprite[i]->SetPosition(TaskPos);
		TasksSprite[i]->SetSize(TaskScl);
	TasksSprite[i]->setcolor({ 1.f,1.f,1.f,TaskSpriteAlpha[i] });
	}
	TasksSprite[1]->SetSize({ TaskScl.x / 1.6f,TaskScl.y });

	TaskClear(Tasks::TASK_ONE, Tasks::TASK_TWO, Judg[Tasks::TASK_ONE],2);
	TaskClear(Tasks::TASK_TWO, Tasks::TASK_THREE, Judg[Tasks::TASK_TWO],3);
	//int->stringに
	if(tasks==Tasks::TASK_THREE)
	{
		TaskAllClear = true;
	}
	
	std::ostringstream str;
	if (tasks == Tasks::TASK_ONE) {
		str << std::fixed << std::setprecision(2)
			<< GolemDestCount;
	}
	if (tasks == Tasks::TASK_TWO) {
		str << std::fixed << std::setprecision(2)
			<< FlogDestCount;
	}

	const std::string amount = "/2";
	DebugTextSprite2::GetInstance()->Print(str.str()+amount,TaskMenuPos.x,TaskMenuPos.y , 0.8);
	int nearIndex_Golem = TargetMarker::GetInstance()->GetNearGolemIndex();
	int nearIndex_Lizard = TargetMarker::GetInstance()->GetNearLizardIndex();

	Player* player = PlayerControl::GetInstance()->GetPlayer();
	Enemy* targetenemy_Golem = EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[nearIndex_Golem].get();
	Enemy* targetenemy_Lizard = EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[nearIndex_Lizard].get();

	switch (target)
	{
	case GOLEM:
		TargetPos= targetenemy_Golem->GetPosition();
		arrowcol = { 0.2,0.2,0.9,0.7 };
		break;

	case COW:
		TargetPos = targetenemy_Lizard->GetPosition();
		arrowcol = { 0.9,0.2,0.2,0.7 };
		break;
	case CHEST:
		if (ChestControl::GetInstance()->ChestCount() == 1) {
			TargetPos = ChestControl::GetInstance()->GetChest(ChestControl::RED)->GetPosition();
		}
		if (ChestControl::GetInstance()->ChestCount() == 2) {
			TargetPos = ChestControl::GetInstance()->GetChest(ChestControl::BLUE)->GetPosition();
		}
		arrowcol = { 0.7,0.7,0.1,0.7 };
		break;
	case Bossarea:
		TargetPos = { 17, -35, 820 };
		break;
	}

	if(Judg[Tasks::TASK_ONE]==false)
	{
		target = Target::GOLEM;
	}
	if(Judg[Tasks::TASK_TWO]==false&& Judg[Tasks::TASK_ONE] == true)
	{
		target = Target::COW;
	}
	if(Judg[Tasks::TASK_TWO] == true && Judg[Tasks::TASK_ONE] == true)
	{
		target = Target::Bossarea;
	}
	//敵がプエレイヤーの方向く処理
	XMVECTOR positionA = {
		player->GetPosition().x,
		player->GetPosition().y,
		player->GetPosition().z
	};
	XMVECTOR positionB = { TargetPos.x, TargetPos.y, TargetPos.z };
	//プレイヤーと敵のベクトルの長さ(差)を求める
	XMVECTOR SubVector = XMVectorSubtract(positionB, positionA); // positionA - positionB;

	float RotY;
	//角度の取得 プレイヤーが敵の索敵位置に入ったら向きをプレイヤーの方に
	RotY = atan2f(SubVector.m128_f32[0], SubVector.m128_f32[2]);
//	if (enemy->GetFbxTime()< 510.000f / 60.000f) {
	curr = 55.f;
	TargetArrowRot={
		90,
		0,
		RotY*-curr+180 ,
		};

	TargetArrow->SetRotation(TargetArrowRot);
	TargetArrow->SetPosition({ player->GetPosition().x,player->GetPosition().y + 0.0f,player->GetPosition().z });
	TargetArrow->SetScale({3,4,3});
	TargetArrow->SetBillboard(FALSE);
	TargetArrow->SetUVMove(TRUE);
	TargetArrow->SetColor(arrowcol);
	TargetArrow->Update(CameraControl::GetInstance()->GetCamera());

	TaskSequence();
	for(int i=0;i<4;i++)
	{

		TargetIcon[i]->SetBillboard(TRUE);
		TargetIcon[i]->SetColor({ 1,1,1,iconalpha[i]});
		TargetIcon[i]->SetScale({ 3,3,3 });
		TargetIcon[i]->SetPosition({ player->GetPosition().x,player->GetPosition().y + 7.0f,player->GetPosition().z });

		TargetIcon[i]->Update(CameraControl::GetInstance()->GetCamera());
	}
}

bool Task::ClearTaskONE()
{
	if(tasks==Tasks::TASK_TWO)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Task::TargetDraw()
{
	Texture::PreDraw();
	TargetArrow->Draw();
	for(int i=0;i<4;i++)
	{
		TargetIcon[i]->Draw();
	}
	Texture::PostDraw();
	
}

void Task::Draw()
{
	
	Sprite::PreDraw();
	TaskFrame->Draw();
	for(int i=0;i<TaskNum;i++)
	{
		TasksSprite[i]->Draw();
	}
	navChestSprite[0]->Draw();
	navChestSprite[1]->Draw();
	DebugTextSprite2::GetInstance()->DrawAll();
	Sprite::PostDraw();

	ImGui::Begin("task");
	ImGui::SliderFloat("X", &TaskMenuPos.x, 0, 1900);
	ImGui::SliderFloat("Y", &TaskMenuPos.y, 0, 1200);
	ImGui::Begin("Frame");
	ImGui::SliderFloat("FX", &navTaskPos.x, 0, 1900);
	ImGui::SliderFloat("FY", &navTaskPos.y, 0, 1200);
	ImGui::SliderFloat("FScaleX", &navTaskScl.x, 0, 1900);
	ImGui::SliderFloat("FScaleY", &navTaskScl.y, 0, 1200);
	ImGui::SliderFloat("FX32", &FramePos.x, 0, 1900);
	ImGui::SliderFloat("FY2", &FramePos.y, 0, 1200);
	ImGui::SliderFloat("FScaleX2", &FrameScl.x, 0, 1900);
	ImGui::SliderFloat("FScaleY2", &FrameScl.y, 0, 1200);


	ImGui::End();

}
void Task::TaskSequence()
{
	Player* l_player = PlayerControl::GetInstance()->GetPlayer();
	if (l_player == nullptr || l_player->GetStopFlag())return;

	Input* input = Input::GetInstance();
	
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
	bool taskclear4 = ChestControl::GetInstance()->ChestCount() == 2;
	if(taskclear0)
	{
		iconalpha[0] -= 0.02f;
	}
	if (iconalpha[0]<=0.0f&&!taskclear1)
	{
		iconalpha[1] += 0.02f;
	}
	if(taskclear1&&!taskclear2)
	{
		iconalpha[1] -= 0.02f;
		
	}
	if(taskclear2&&!taskclear3)
	{
		if (iconalpha[1] <= 0.0f)
		{
			iconalpha[2] += 0.02f;
			//	iconalpha[3] += 0.02f;
		}
	}
	if(taskclear3)
	{
		iconalpha[2] -= 0.02f;
	}
	if (taskclear3&&taskclear4)
	{
		if (iconalpha[2] <= 0.0f)
		{
			iconalpha[3] += 0.02f;
			//	iconalpha[3] += 0.02f;
		}
	}
	if (taskclear4)
	{
		if (movement > 460) {
			iconalpha[3] -= 0.02f;
		}
	}
	for(int i=0;i<4;i++)
	{
		iconalpha[i] = min(iconalpha[i], 1.0f);
		iconalpha[i] = max(iconalpha[i], 0.0f);
	}
}

void Task::TaskClear(const Tasks& task, const Tasks& nexttask, bool clearjudg, int chestcount)
{
	if (tasks != task)return;
	if(clearjudg)
	{
		if (ChestControl::GetInstance()->ChestCount() == chestcount) {
			TaskScl.x += 5.0f;
			TaskScl.y += 5.0f;
			navSpriteAlpha[0] -= 0.05f;
			navSpriteAlpha[1] += 0.05f;
			TaskSpriteAlpha[task] -= 0.05f;
			if (TaskSpriteAlpha[task] <= 0.0f) {
				TaskSpriteAlpha[nexttask] = 1.0f;
				tasks = nexttask;
			}
		}
		else
		{
			target = Target::CHEST;
			navSpriteAlpha[1] -= 0.02f;
			navSpriteAlpha[0] += 0.02f;
		}
	}

	for(int i=0;i<2;i++)
	{
		navSpriteAlpha[i] = min(navSpriteAlpha[i], 1.f);
		navSpriteAlpha[i] = max(navSpriteAlpha[i], 0.f);
	}

}


