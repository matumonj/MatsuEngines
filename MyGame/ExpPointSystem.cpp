#include "ExpPointSystem.h"
#include"mHelper.h"
#include"imgui.h"
#include"DebugTxt.h"
#include <iomanip>
#include <sstream>
#include"EnemyControl.h"

ExpPointSystem* ExpPointSystem::GetIns()
{
	static ExpPointSystem instance;
	return &instance;
}

void ExpPointSystem::ExpPoint_Get(int point)
{
}

void ExpPointSystem::ExpPoint_Los(int point)
{
	m_ExpPoint -= point;
}

void ExpPointSystem::Init()
{
}

void ExpPointSystem::Upda()
{
}

void ExpPointSystem::expBarUpda()
{
}


void ExpPointSystem::Draw()
{
	//ImGui::Begin("BossHP");
	//ImGui::SliderFloat("BarX", &BarScl.x, 0, 1900);
	//ImGui::SliderFloat("BarY", &BarScl.y, 0, 1000);
	//ImGui::SliderFloat("BarPosX", &BarPos.x, 0, 1000);
	//ImGui::SliderFloat("BarPosY", &BarPos.y, 0, 900);
	//ImGui::End();

	//ImGui::Begin("BossHPFrame");
	//ImGui::SliderFloat("FrameSclX", &BarFrameScl.x, 0, 1900);
	//ImGui::SliderFloat("FrameSclY", &BarFrameScl.y, 0, 1000);
	//ImGui::SliderFloat("FramePosX", &BarFramePos.x, 0, 1000);
	//ImGui::SliderFloat("FramePosY", &BarFramePos.y, 0, 900);
	//ImGui::End();
}
