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
		
		SelectSword::GetInstance()->Draw();
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL)
	{
		TutorialSprite::GetInstance()->Draw();
	}

	EnemyControl::GetInstance()->HPFrameDraw();
	if (!TurnOffDrawUI)
	{
		if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY)
		{
			DropWeapon::GtIns()->Draw_PickTex();
			Task::GetInstance()->Draw();

			for (int i = 0; i < EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE).size(); i++)
			{
				if (EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i] == nullptr)
				{
					continue;
				}
				EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE)[i]->DamageTexDisplay_Draw();
			}
			if (EnemyControl::GetInstance()->GetGuardianEnemy() != nullptr)
			{
				EnemyControl::GetInstance()->GetGuardianEnemy()->DamageTexDisplay_Draw();
			}
		}
	}
	if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS&&
		EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS).size()>0&&
		EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]!=nullptr)
	{
		EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS)[0]->DamageTexDisplay_Draw();

		for (int i = 0; i < 2; i++)
		{
			if (HalfAttack::GetInstance()->GetSummonEnemy(i) == nullptr)continue;
			HalfAttack::GetInstance()->GetSummonEnemy(i)->DamageTexDisplay_Draw();
		}
		HalfAttack::GetInstance()->Draw_SummonEnemyHP();
	}

	//PlayerControl::GetInstance()->DamageTexDraw();
	if (!TurnOffDrawUI)
	{
		PlayerControl::GetInstance()->DamageTexDraw();
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
