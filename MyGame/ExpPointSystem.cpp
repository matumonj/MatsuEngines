#include "ExpPointSystem.h"
#include"mHelper.h"
#include"imgui.h"
#include"DebugTxt.h"
#include <iomanip>
#include <sstream>
#define MAX_LEVEL 10
ExpPointSystem* ExpPointSystem::GetInstance()
{
	static ExpPointSystem instance;
	return &instance;
}
void ExpPointSystem::ExpPoint_Get(int point)
{
	m_ExpPoint += point;
	old += point;
	m_BarSet = true;

}
void ExpPointSystem::ExpPoint_Los(int point)
{
	m_ExpPoint -= point;
}

void ExpPointSystem::Init()
{
	Sprite::LoadTexture(6, L"Resources/HPTex/frame3.png");
	Sprite::LoadTexture(5, L"Resources/HPTex/yellow.png");

	Sprite* l_Bar = Sprite::Create(5, { 0,0 });
	Sprite* l_BarFrame = Sprite::Create(6, { 0,0 });

	m_ExpBarSpite.reset(l_Bar);
	m_ExpBarFrameSprite.reset(l_BarFrame);

	m_ExpBarSpite->SetSize({ 0,20 });
	m_ExpBarFrameSprite->SetSize({ 600,50 });
	m_ExpBarSpite->SetAnchorPoint({ 0.0f,0.0f });
	m_ExpBarFrameSprite->SetAnchorPoint({ 0.0f,0.5f });

	BarPos = { 420.0f,830.0f };
	BarFramePos = { 200.0f,830.0f };
	BarScl = { 0.0f,30.0f };
	BarFrameScl = { 1465.0f,57.0f };

	m_ExpPoint = 0;
	m_Lvel = 0;
	m_LevelUp_needExp = 10;
	LevelUpTexPos = { 720.0f,930.0f };

}
void ExpPointSystem::Upda()
{

	m_LevelUp_needExp = m_Lvel + 10;
	m_LevelUpF = m_ExpPoint >= ol + m_LevelUp_needExp;

	
	expBarUpda();
}
void ExpPointSystem::expBarUpda()
{
	BarSclX_AddValue = ((float)m_ExpPoint - (float)m_OldExpPoint) * ((BarFrameScl.x - 200) / (float)m_LevelUp_needExp);
	if (m_BarSet) {
		BarScl.x += 10.0f;
		if (!m_LevelUpF) {
			if (BarScl.x > OldBarScl.x + BarSclX_AddValue) {
				m_BarSet = false;
			}
		} else {
			if (BarScl.x >= BarFrameScl.x - 200) {
				old = 0;
				m_EaseTime = 0;
				oldGetExp = 0;
				m_Lvel++;
				TextDis = true;
				ol = m_ExpPoint;
				BarScl.x = 0;
			
				m_BarSet = false;
			}
		}
	} else {
		m_OldExpPoint = m_ExpPoint;
		OldBarScl = BarScl;
	
	}
	if (TextDis) {
		LvelUpFont_Display();
	}
	else{

		LvelUpFont_FeedOut();
	}
	m_ExpBarSpite->SetPosition(BarPos);
	m_ExpBarSpite->SetSize(BarScl);

	m_ExpBarFrameSprite->SetPosition(BarFramePos);
	m_ExpBarFrameSprite->SetSize(BarFrameScl);

	DebugTextSprite::GetInstance()->Print("L", 500, 830, fontSize[0]);
	DebugTextSprite::GetInstance()->Print("v", 650, 830, fontSize[1]);
	DebugTextSprite::GetInstance()->Print("e", 800, 830, fontSize[2]);
	DebugTextSprite::GetInstance()->Print("l", 950, 830, fontSize[3]);
	DebugTextSprite::GetInstance()->Print("U", 1100, 830, fontSize[4]);
	DebugTextSprite::GetInstance()->Print("p", 1250, 830, fontSize[5]);
	std::ostringstream str;
	std::ostringstream str2;
	std::ostringstream str3;

	if (!m_BarSet) {

		oldGetExp = old;
	}
	if(oldGetExp !=old) {
		if (m_EaseTime <= 1.0f) {
			m_EaseTime += 0.01f;
		} else {
			//	m_OldExpPoint = m_ExpPoint;
			m_EaseTime = 0;
		}
	}
	int Tex;
	Tex=(int)Easing::EaseOut(m_EaseTime, oldGetExp, old);
	
	str << std::fixed << std::setprecision(2)
		<<Tex;

	str3 << std::fixed << std::setprecision(2)
		<< m_Lvel;
	DebugTextSprite::GetInstance()->Print("Lv", LevelUpTexPos.x-450, LevelUpTexPos.y-70, 0.5);
	DebugTextSprite::GetInstance()->Print(str3.str(), LevelUpTexPos.x - 380, LevelUpTexPos.y-70, 0.8f);
	DebugTextSprite::GetInstance()->Print("exp", LevelUpTexPos.x, LevelUpTexPos.y, 0.5);
	DebugTextSprite::GetInstance()->Print(str.str(), LevelUpTexPos.x+100,LevelUpTexPos.y,0.5);
	DebugTextSprite::GetInstance()->Print("/", LevelUpTexPos.x+200, LevelUpTexPos.y, 0.5);
	str2 << std::fixed << std::setprecision(2)
		<< m_LevelUp_needExp;
	DebugTextSprite::GetInstance()->Print(str2.str(), LevelUpTexPos.x+250, LevelUpTexPos.y, 0.5);

}


void ExpPointSystem::LvelUpFont_Display()
{
	if (fontEaseT[0] < 1.0f) {
		fontEaseT[0] += 0.02f;
	}
	if (fontEaseT[5] >1.0f) {
		TextDis = false;
	}
	for (int i = 1; i < 6; i++) {
		if (m_ReturnEase[i]) {
			continue;
		}
		if (fontEaseT[i - 1] > 0.5f) {
			fontEaseT[i] += 0.05f;
		}
	}

	for (int i = 0; i < 6; i++) {
		if (fontEaseT[i] > 1.0f) {
			m_ReturnEase[i] = true;
		}
		fontSize[i] = Easing::EaseOut(fontEaseT[i], 0, 2);
	}
	DebugTextSprite::GetInstance()->FeedTex();
}

void ExpPointSystem::LvelUpFont_FeedOut()
{

		for (int i = 0; i < 6; i++) {
			if (m_ReturnEase[i]) {
				fontEaseT[i] -= 0.05f;
				if (fontEaseT[i] < 0.0f) {
					m_ReturnEase[i] = false;

				}
			}
			
			fontSize[i] = Easing::EaseOut(fontEaseT[i], 0, 2);
			fontEaseT[i] = max(fontEaseT[i], 0.0f);
		}
		
}
void ExpPointSystem::Draw()
{
	Sprite::PreDraw();
	m_ExpBarFrameSprite->Draw();
	m_ExpBarSpite->Draw();
	DebugTextSprite::GetInstance()->DrawAll();
	Sprite::PostDraw();
	int num;
	if (TextDis == true) {
		num = 1;
	}
	else {
		num = 0;
	}
}