#include "UI.h"
#include"SceneManager.h"
#include"EnemyControl.h"
#include"TutorialSprite.h"
#include"BossSpell.h"
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
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		TutorialSprite::GetInstance()->Initialize();
	}
}

void UI::HUDUpdate(bool& hudload, DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		HUD::GetInstance()->EnemyHPGauge_MultiUpdate(hudload, camera,
		                                             EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL));
		TutorialSprite::GetInstance()->Update();
	}
	BossSpell::GetInstance()->Update();
	HUD::GetInstance()->SkillBottonUpdate();
	HUD::GetInstance()->Update();
	HUD::GetInstance()->TaskUpdate(camera);

	ExpPointSystem::GetInstance()->Upda();
}

void UI::HUDDraw()
{
	if (!TurnOffDrawUI)
	{
		HUD::GetInstance()->SkillBottonDraw();
		ExpPointSystem::GetInstance()->Draw();
		if (TutorialSprite::GetInstance()->GetClearSetting())
		{
			HUD::GetInstance()->EnemyHPGauge_MultiDraw();
		}

		BossSpell::GetInstance()->Draw();
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		TutorialSprite::GetInstance()->Draw();
	}
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
