#include "UI.h"
#include"SceneManager.h"
#include"EnemyControl.h"
#include"TutorialSprite.h"
#include"BossSpell.h"
#include"EncountGuardianSprite.h"
#include"ExpPointSystem.h"
UI* UI::GetInstance()
{
	static UI instance;
	return &instance;
}
void UI::Initialize()
{
	ExpPointSystem::GetInstance()->Init();
	BossSpell::GetInstance()->Initialize();
	HUD::GetInstance()->Initialize();
	HUD::GetInstance()->EnemyHPGaugeInitialize();
	HUD::GetInstance()->SkillButtonInitialize();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		TutorialSprite::GetInstance()->Initialize();
	}
	EncountGuardianSprite::GetInstance()->Init();
}

void UI::HUDUpdate(bool&hudload, DebugCamera* camera)
{
	//if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
	//	HUD::GetInstance()->EnemyHPGaugeUpdate(EnemyControl::GetInstance()->GetEnemyindex(0));
	//	HUD::GetInstance()->EnemyHPGauge_MultiUpdate(hudload, camera, EnemyControl::GetInstance()->GetEnemyindex(0));
	//}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
	//	
	//	HUD::GetInstance()->EnemyHPGaugeUpdate(EnemyControl::GetInstance()->GetTutorialEnemyindex());
		HUD::GetInstance()->EnemyHPGauge_MultiUpdate(hudload, camera, EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL));
	TutorialSprite::GetInstance()->Update();
	//
	}
	BossSpell::GetInstance()->Update();
	HUD::GetInstance()->SkillBottonUpdate();
	HUD::GetInstance()->Update();
	HUD::GetInstance()->TaskUpdate(camera);

	EncountGuardianSprite::GetInstance()->Update();

	ExpPointSystem::GetInstance()->Upda();
}

void UI::HUDDraw()
{
	if (TurnOffDrawUI)return;
	if (TutorialSprite::GetInstance()->GetClearSetting()) {
		HUD::GetInstance()->EnemyHPGauge_MultiDraw();
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
		TutorialSprite::GetInstance()->Draw();
	}
	
	HUD::GetInstance()->SkillBottonDraw();
	BossSpell::GetInstance()->Draw();

	EncountGuardianSprite::GetInstance()->Draw();
	ExpPointSystem::GetInstance()->Draw();
}

void UI::AreaNameDraw()
{
	HUD::GetInstance()->AreaName();
}
void UI::Finalize()
{
	TutorialSprite::GetInstance()->Finalize();
	HUD::GetInstance()->Finalize();
}