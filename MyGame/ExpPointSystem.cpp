#include "ExpPointSystem.h"
#include"mHelper.h"
#include"imgui.h"
#include"DebugTxt.h"
#include <iomanip>
#include <sstream>
#include"EnemyControl.h"

ExpPointSystem* ExpPointSystem::GetInstance()
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
	Sprite::LoadTexture(6, L"Resources/HPTex/frame3.png");
	Sprite::LoadTexture(5, L"Resources/HPTex/yellow.png");

	Sprite* l_Bar = Sprite::Create(5, {0, 0});
	Sprite* BossHPFrame = Sprite::Create(6, {0, 0});

	m_BossHP.reset(l_Bar);
	m_BossHPFrame.reset(BossHPFrame);

	m_BossHP->SetSize({0, 20});
	m_BossHPFrame->SetSize({600, 50});
	m_BossHP->SetAnchorPoint({0.0f, 0.0f});
	m_BossHPFrame->SetAnchorPoint({0.0f, 0.5f});

	BarPos = {381.0f, 862.0f};
	BarFramePos = {122.0f, 830.0f};
	BarScl = {
		Percent::GetParcent(
			static_cast<float>(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetMaxHP()),
			static_cast<float>(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetHP())) * 15.35f,
		51.0f
	};
	BarFrameScl = {
		static_cast<float>(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetMaxHP()) * 6.8f, 117.0f
	};
}

void ExpPointSystem::Upda()
{
	if (m_BossHPFrame == nullptr)
	{
		return;
	}
	if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0] == nullptr)
	{
		return;
	}
	if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetRecvDamage())
	{
		NowHP = Percent::GetParcent(
			static_cast<float>(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetMaxHP()),
			static_cast<float>(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetHP()));
		m_EaseTime += 0.05f;
		BarScl.x = Easing::EaseOut(m_EaseTime, OldHP * 15.35f, NowHP * 15.35f);

		if (m_EaseTime >= 1.0f)
		{
			EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->SetRecvDamage(FALSE);
		}
	}

	else
	{
		OldHP = Percent::GetParcent(static_cast<float>(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetMaxHP()),
			static_cast<float>(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->GetHP()));
		m_EaseTime = 0.0f;
	}
	expBarUpda();
}

void ExpPointSystem::expBarUpda()
{
	m_BossHP->SetPosition(BarPos);
	m_BossHP->SetSize(BarScl);

	m_BossHPFrame->SetPosition(BarFramePos);
	m_BossHPFrame->SetSize(BarFrameScl);
}


void ExpPointSystem::Draw()
{
	if (m_BossHPFrame == nullptr)
	{
		return;
	}
	Sprite::PreDraw();

	m_BossHP->Draw();
	m_BossHPFrame->Draw();
	Sprite::PostDraw();

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
