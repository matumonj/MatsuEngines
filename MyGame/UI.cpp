#include "UI.h"
#include"SceneManager.h"
#include"EnemyControl.h"
#include"TutorialSprite.h"
#include"BossSpell.h"
#include"ExpPointSystem.h"
#include"Task.h"
#include"DropWeapon.h"
#include <SelectSword.h>

UI* UI::GetInstance()
{
	static UI instance;
	return &instance;
}

void UI::Initialize()
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
	}
	BossSpell::GetInstance()->Initialize();
	HUD::GetInstance()->Initialize();
	HUD::GetInstance()->EnemyHPGaugeInitialize();
	HUD::GetInstance()->SkillButtonInitialize();
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		TutorialSprite::GetInstance()->Initialize();
	}
	DropWeapon::GtIns()->Init();
	Task::GetInstance()->Init();
}

void UI::HUDUpdate(bool& hudload, DebugCamera* camera)
{
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		HUD::GetInstance()->EnemyHPGauge_MultiUpdate(hudload, camera,
		                                             EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL));
		TutorialSprite::GetInstance()->Update();
	}
	HUD::GetInstance()->SkillBottonUpdate();
	HUD::GetInstance()->Update();
	HUD::GetInstance()->TaskUpdate(camera);
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
	{
		ExpPointSystem::GetInstance()->Upda();
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
	{
		Task::GetInstance()->Upda();
	}
}

void UI::HUDDraw()
{
	if (!TurnOffDrawUI)
	{
		HUD::GetInstance()->SkillBottonDraw();
		if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS)
		{
			ExpPointSystem::GetInstance()->Draw();
		}
		if (TutorialSprite::GetInstance()->GetClearSetting())
		{
			HUD::GetInstance()->EnemyHPGauge_MultiDraw();
		}

		SelectSword::GetInstance()->Draw();
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		EnemyControl::GetInstance()->HPFrameDraw();
		TutorialSprite::GetInstance()->Draw();
	}

	if (!TurnOffDrawUI)
	{
		if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
		{
			DropWeapon::GtIns()->Draw_PickTex();
			Task::GetInstance()->Draw();
		}
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
