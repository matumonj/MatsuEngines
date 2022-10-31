#include "ExpPointSystem.h"
#include"mHelper.h"
#include"imgui.h"
#define MAX_LEVEL 10
ExpPointSystem* ExpPointSystem::GetInstance()
{
	static ExpPointSystem instance;
	return &instance;
}
void ExpPointSystem::ExpPoint_Get(int point)
{
	m_ExpPoint += point; 
	m_BarSet = true;

}
void ExpPointSystem::ExpPoint_Los(int point)
{ 
	m_ExpPoint -= point; 
}

void ExpPointSystem::Init()
{
	Sprite::LoadTexture(6, L"Resources/HPTex/frame.png");
	Sprite::LoadTexture(5, L"Resources/HPTex/HPgauge.png");

	Sprite* l_Bar = Sprite::Create(5, { 0,0 });
	Sprite* l_BarFrame = Sprite::Create(6, { 0,0 });

	m_ExpBarSpite.reset(l_Bar);
	m_ExpBarFrameSprite.reset(l_BarFrame);

	m_ExpBarSpite->SetSize({ 0,20 });
	m_ExpBarFrameSprite->SetSize({ 600,50 });
	m_ExpBarSpite->SetAnchorPoint({ 0.0f,0.5f });
	m_ExpBarFrameSprite->SetAnchorPoint({ 0.0f,0.5f });

	BarPos = { 420.0f,830.0f };
	BarFramePos = { 200.0f,830.0f };
	BarScl = { 0.0f,57.0f };
	BarFrameScl = { 1465.0f,57.0f };

	m_ExpPoint = 0;
	m_Lvel = 0;
	m_LevelUp_needExp = 10;


}
void ExpPointSystem::Upda()
{
	m_LevelUpF = m_ExpPoint >= m_LevelUp_needExp;
	if (m_Lvel != 0) {
		m_LevelUp_needExp = m_Lvel + 10;
	}
	expBarUpda();
}
void ExpPointSystem::expBarUpda()
{
	BarSclX_AddValue=((float)m_ExpPoint- (float)m_OldExpPoint )* (BarFrameScl.x / (float)m_LevelUp_needExp);
	if (m_BarSet) {
		BarScl.x += 10.0f;
		if (!m_LevelUpF) {
		if (BarScl.x > OldBarScl.x + BarSclX_AddValue) {
				m_BarSet = false;
			}
		}
		else {
			if (BarScl.x >= BarFrameScl.x-50) {
				BarScl.x = 0;
				m_BarSet = false;
			}
		}
	}
	else {
		m_OldExpPoint = m_ExpPoint;
		OldBarScl = BarScl;
	}

	m_ExpBarSpite->SetPosition(BarPos);
	m_ExpBarSpite->SetSize(BarScl);
	
	m_ExpBarFrameSprite->SetPosition(BarFramePos);
	m_ExpBarFrameSprite->SetSize(BarFrameScl);
}


void ExpPointSystem::Draw()
{
	Sprite::PreDraw();
	m_ExpBarFrameSprite->Draw();
	m_ExpBarSpite->Draw();
	Sprite::PostDraw();
	ImGui::Begin("Bar_frame");
	ImGui::SliderFloat("BarPosX", &BarPos.x, 0, 1900);
	ImGui::SliderFloat("BarPosY", &BarPos.y, 0, 1000);
	ImGui::SliderFloat("FrameBarPosX", &BarFramePos.x, 0, 1900);
	ImGui::SliderFloat("FrameBarPosY", &BarFramePos.y, 0, 1000);
	ImGui::SliderFloat("BarSclX", &BarScl.x, 0, 1900);
	ImGui::SliderFloat("BarSclY", &BarScl.y, 0, 1000);
	ImGui::SliderInt("FrameBarSclX", &m_ExpPoint, 0, 1900);
	ImGui::SliderFloat("FrameBarSclY", &BarSclX_AddValue, 0, 1000);
}