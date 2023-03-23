#include "UI.h"

#include <algorithm>

#include"SceneManager.h"
#include"EnemyControl.h"
#include"TutorialSprite.h"
#include"Task.h"
#include"DropWeapon.h"
#include <SelectSword.h>
#include "HalfAttack.h"
#include "ImageManager.h"
#include "mHelper.h"
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
	HUD::GetIns()->Initialize();
	HUD::GetIns()->EnemyHPGaugeInitialize();
	HUD::GetIns()->SkillButtonInitialize();
	if (SceneManager::GetIns()->GetScene() == SceneManager::TUTORIAL)
	{
		AreaSel[BOSS].reset(Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::AREASELECT_BOSS), { 0.f,0.f }));
		AreaSel[EXPLO].reset(Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::AREASELECT_EXPLO), { 0.f,0.f }));
		AreaSel[FRAME].reset(Sprite::Create(ImageManager::GetIns()->GetImage(ImageManager::AREASELECT_FRAME), { 0.f,0.f }));

		TutorialSprite::GetIns()->Initialize();
	}
	DropWeapon::GtIns()->Init();
	Task::GetIns()->Init();
}

void UI::AreaSelectTexInit()
{
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
	
	if (SceneManager::GetIns()->GetScene() == SceneManager::PLAY)
	{
		Task::GetIns()->Upda();
	}
}

void UI::AreaSelectTexUpda()
{
	
	AreaSel[EXPLO]->SetPosition({ 800.f,500.f });
	AreaSel[BOSS]->SetPosition({ 1200.f,500.f });

	SelectScene_Open();
	SelectScene_Close();

	//切り替え始まったらウィンドウ閉じる
	if (SceneChange_Explo || SceneChange_Boss)
	{
		for (auto i = 0; i < AreaSel.size(); i++)
		{
			AreaSel_EaseC[i] -= AddEaseTime;
		}
	}
	if (SelectF&&Input::GetIns()->TriggerButton(Input::A))
	{
		if (index == EXPLO)
		{
			SceneChange_Explo = true;
		}
		if (index == BOSS)
		{
			SceneChange_Boss = true;
		}
	}

	
}

void UI::SelectScene_Open()
{
	if (SceneChange_Explo || SceneChange_Boss)
	{
		return;
	}
	if (!SelectF)return;
	for (auto i = 0; i < AreaSel.size(); i++)
	{
		if (AreaSel[i] == nullptr)continue;
		AreaSel_EaseC[i] += AddEaseTime;
	}

	if (Input::GetIns()->TriggerButton(Input::X))
	{
		index--;
	}
	if (Input::GetIns()->TriggerButton(Input::B))
	{
		index++;
	}

	//もう一度Startでウィンドウ閉じる
	if (AreaSel_EaseC[0] > 0.9f)
	{
		if (Input::GetIns()->TriggerButton(Input::START))
			SelectF = false;
	}

	AreaSel[FRAME]->SetPosition({ FrameX[index],500.f });
}

void UI::SelectScene_Close()
{
	if (SceneChange_Explo || SceneChange_Boss)
	{
		return;
	}
	if (SelectF)return;

	if (AreaSel_EaseC[0] < 0.1f)
	{
		if (Input::GetIns()->TriggerButton(Input::START))
			SelectF = true;
	}
	for (auto i = 0; i < AreaSel.size(); i++)
	{
		if (AreaSel[i] == nullptr)continue;
		AreaSel_EaseC[i] -= AddEaseTime;

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
			Sprite::PreDraw();
			for (auto i = 0; i < AreaSel.size(); i++)
			{
				if (AreaSel[i] == nullptr)continue;

				AreaSel[i]->SetAnchorPoint({ 0.5f,0.5f });

				AreaSel_EaseC[i] = std::clamp(AreaSel_EaseC[i], 0.f, 1.f);

				AreaSel_Size[i] = Easing::EaseOut(AreaSel_EaseC[i], 0.f, 400.f);

				AreaSel[i]->SetSize({ AreaSel_Size[i],AreaSel_Size[i] });
				AreaSel[i]->Draw();
			}
			Sprite::PostDraw();
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
	constexpr float BlurAddVal = 0.02f;

	constexpr float MaxBlurSmp = 90.f;

	if (BlurF) {
		EaseT += BlurAddVal;
		if (EaseT > 1.f)
		{
			BlurF = false;
		}
	} else
	{
		EaseT -= BlurAddVal;
	}

	EaseT = std::clamp(EaseT, 0.f, 1.f);
	bloomval = Easing::EaseOut(EaseT, 0.f, MaxBlurSmp);
}


void UI::TwistEffect(bool f, float radiusAddVal, float PowerAddVal)
{
	if (f)
	{
		TwistRad += radiusAddVal;
		TwistPower += PowerAddVal; 
	}
	TwistRad = std::clamp(TwistRad, 0.f, 1900.f);
	TwistPower = std::clamp(TwistPower, 0.f, 30.f);
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
