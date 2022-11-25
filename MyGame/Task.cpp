#include "Task.h"
#include"DebugTxt.h"
#include"EnemyControl.h"
#include"imgui.h"
Task* Task::GetInstance()
{
	static Task ins;
	return&ins;
}

void Task::Init()
{
	Sprite::LoadTexture(19, L"Resources/2d/LevelUp/debugfont - コピー.png");
	Sprite::LoadTexture(20, L"Resources/03 開閉型メニュー/bg_menu_tab.png");
	Sprite::LoadTexture(21, L"Resources/2d/Frame/Task1.png");
	Sprite::LoadTexture(22, L"Resources/2d/Frame/Task2.png");
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
	//TasksSprite[Tasks::TASK_ONE].reset(Sprite::Create(20, { 0,0 }));
	TaskFrame->SetAnchorPoint({ 0.5f,0.5f });
	for(int i=0;i<TaskNum;i++)
	{
		TasksSprite[i]->SetAnchorPoint({ 0.5,0.5 });
	}
}

void Task::Upda()
{
	Judg[Tasks::TASK_ONE] = GolemDestCount > 5;
	Judg[Tasks::TASK_TWO] = FlogDestCount > 5;

	TaskFrame->SetPosition(FramePos);
	TaskFrame->SetSize(FrameScl);
	TaskClear(Tasks::TASK_ONE, Judg[Tasks::TASK_ONE]);
	TaskClear(Tasks::TASK_TWO, Judg[Tasks::TASK_TWO]);
	//int->stringに
	std::ostringstream str;
	str << std::fixed << std::setprecision(2)
		<<GolemDestCount;

	const std::string amount = "/5";
	DebugTextSprite2::GetInstance()->Print(str.str()+amount,TaskMenuPos.x,TaskMenuPos.y , 1);
}

void Task::Draw()
{
	Sprite::PreDraw();
	TaskFrame->Draw();
	DebugTextSprite2::GetInstance()->DrawAll();
	Sprite::PostDraw();

	ImGui::Begin("task");
	ImGui::SliderFloat("X", &TaskMenuPos.x, 0, 1900);
	ImGui::SliderFloat("Y", &TaskMenuPos.y, 0, 1200);
	ImGui::Begin("Frame");
	ImGui::SliderFloat("FX", &FramePos.x, 0, 1900);
	ImGui::SliderFloat("FY", &FramePos.y, 0, 1200);
	ImGui::SliderFloat("FScaleX", &FrameScl.x, 0, 1900);
	ImGui::SliderFloat("FScaleY", &FrameScl.y, 0, 1200);

	ImGui::End();

}

void Task::TaskClear(const Tasks& task, bool clearjudg)
{
	
}
