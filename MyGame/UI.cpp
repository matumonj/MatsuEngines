#include "UI.h"
#include"SceneManager.h"
#include"EnemyControl.h"
#include"TutorialSprite.h"
#include"BossSpell.h"
#include"ExpPointSystem.h"
#include"Task.h"
#include"DropWeapon.h"
#include <SelectSword.h>

#include "GameOver.h"
#include "HalfAttack.h"
#include "PlayerControl.h"

UI* UI::GetIns()
{
	static UI instance;
	return &instance;
}

void UI::Initialize()
{
	if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{
	}
	BossSpell::GetIns()->Initialize();
	HUD::GetIns()->Initialize();
	HUD::GetIns()->EnemyHPGaugeInitialize();
	HUD::GetIns()->SkillButtonInitialize();
	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	{
		TutorialSprite::GetIns()->Initialize();
	}
	DropWeapon::GtIns()->Init();
	Task::GetIns()->Init();
}

void UI::HUDUpdate(bool& hudload, DebugCamera* camera)
{
	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	{
		HUD::GetIns()->EnemyHPGauge_MultiUpdate(hudload, camera,
		                                        EnemyControl::GetIns()->GetEnemy(EnemyControl::TUTORIAL));
		TutorialSprite::GetIns()->Update();
	}
	HUD::GetIns()->SkillBottonUpdate();
	HUD::GetIns()->Update();
	HUD::GetIns()->TaskUpdate(camera);
	if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS)
	{
		ExpPointSystem::GetIns()->Upda();
	}
	if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	{
		Task::GetIns()->Upda();
	}
}

void UI::HUDDraw()
{
	if (!TurnOffDrawUI)
	{
		HUD::GetIns()->SkillBottonDraw();

		SelectSword::GetIns()->Draw();
	}
	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	{
		TutorialSprite::GetIns()->Draw();
	}

	EnemyControl::GetIns()->HPFrameDraw();
	if (!TurnOffDrawUI)
	{
		if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
		{
			DropWeapon::GtIns()->Draw_PickTex();
			Task::GetIns()->Draw();

			for (int i = 0; i < EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE).size(); i++)
			{
				if (EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr)
				{
					continue;
				}
				EnemyControl::GetIns()->GetEnemy(EnemyControl::PLAYSCENE)[i]->DamageTexDisplay_Draw();
			}
			if (EnemyControl::GetIns()->GetGuardianEnemy() != nullptr)
			{
				EnemyControl::GetIns()->GetGuardianEnemy()->DamageTexDisplay_Draw();
			}
		}
	}
	if (SceneManager::GetIns()->GetScene() == SceneManager::BOSS &&
		EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS).size() > 0 &&
		EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0] != nullptr)
	{
		EnemyControl::GetIns()->GetEnemy(EnemyControl::BOSS)[0]->DamageTexDisplay_Draw();

		for (int i = 0; i < 2; i++)
		{
			if (HalfAttack::GetIns()->GetSummonEnemy(i) == nullptr)
			{
				continue;
			}
			HalfAttack::GetIns()->GetSummonEnemy(i)->DamageTexDisplay_Draw();
		}
		HalfAttack::GetIns()->Draw_SummonEnemyHP();
	}

	//PlayerControl::GetIns()->DamageTexDraw();
	if (!TurnOffDrawUI)
	{
		PlayerControl::GetIns()->DamageTexDraw();
	}
}

void UI::AreaNameDraw()
{
	HUD::GetIns()->AreaName();
}

void UI::Finalize()
{
	TutorialSprite::GetIns()->Finalize();
	HUD::GetIns()->Finalize();
}
