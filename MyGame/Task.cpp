#include "Task.h"
#include"DebugTxt.h"
#include"EnemyControl.h"
#include"imgui.h"
#include"ChestControl.h"
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
	Sprite::LoadTexture(20, L"Resources/03 開閉型メニュー/bg_menu_tab.png");
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
		FramePos = { 1695.0f,775.0f };
		FrameScl = { 410.0f,650.0f };

		TaskPos = { 1725.0f,720.0f };
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
		navTaskPos = { 1724.0f,515.0f };
		navTaskScl = { 320.0f,220.0f };
		navChestSprite[0].reset(Sprite::Create(17, {0,0}));
		navChestSprite[1].reset(Sprite::Create(18, {0,0}));
		navChestSprite[0]->SetAnchorPoint({0.5f,0.5f});
		navChestSprite[1]->SetAnchorPoint({ 0.5f,0.5f });
	}
	navSpriteAlpha[0] = 0.0f;
	navSpriteAlpha[1] = 1.0f;
	TaskMenuPos = { 1685.0f,915.0f };
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
	TasksSprite[1]->SetSize({ TaskScl.x * 4.0f / 3.0f,TaskScl.y });

	TaskClear(Tasks::TASK_ONE, Tasks::TASK_TWO, Judg[Tasks::TASK_ONE],1);
	TaskClear(Tasks::TASK_TWO, Tasks::TASK_THREE, Judg[Tasks::TASK_TWO],2);
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
	DebugTextSprite2::GetInstance()->Print(str.str()+amount,TaskMenuPos.x,TaskMenuPos.y , 1);
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

	/*ImGui::Begin("task");
	ImGui::SliderFloat("X", &TaskMenuPos.x, 0, 1900);
	ImGui::SliderFloat("Y", &TaskMenuPos.y, 0, 1200);
	ImGui::Begin("Frame");
	ImGui::SliderFloat("FX", &TaskPos.x, 0, 1900);
	ImGui::SliderFloat("FY", &TaskPos.y, 0, 1200);
	ImGui::SliderFloat("FScaleX", &TaskScl.x, 0, 1900);
	ImGui::SliderFloat("FScaleY", &TaskScl.y, 0, 1200);*/

	//ImGui::End();

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


