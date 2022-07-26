#include "UI.h"
#include"EnemyControl.h"
UI* UI::GetInstance()
{
	static UI instance;
	return &instance;
}
void UI::Initialize()
{
	HUD::GetInstance()->Initialize();
	HUD::GetInstance()->EnemyHPGaugeInitialize();
	HUD::GetInstance()->SkillButtonInitialize();
}

void UI::HUDUpdate(bool&hudload, DebugCamera* camera)
{
	Subenemys.resize(EnemyControl::GetInstance()->GetQuentity());

	
	HUD::GetInstance()->EnemyHPGaugeUpdate(EnemyControl::GetInstance()->GetEnemyindex(0));
	HUD::GetInstance()->EnemyHPGauge_MultiUpdate(hudload, camera, EnemyControl::GetInstance()->GetEnemyindex(0));
	HUD::GetInstance()->SkillBottonUpdate();
	HUD::GetInstance()->Update();

}

void UI::HUDDraw()
{
	HUD::GetInstance()->EnemyHPGauge_MultiDraw();
	HUD::GetInstance()->Draw();
	HUD::GetInstance()->SkillBottonDraw();

}