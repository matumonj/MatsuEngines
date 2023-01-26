#include "PlayerAttackState.h"
#include"Input.h"
#include"EnemyControl.h"
#include"PlayerControl.h"
#include"SelectSword.h"
#include"AttackCollision.h"
#include"SceneManager.h"
#include"HUD.h"

/*後でplayerに移して消す*/
PlayerAttackState* PlayerAttackState::GetInstance()
{
	static PlayerAttackState instance;

	return &instance;
}

void PlayerAttackState::ComboAction()
{
	if (OldSkill == First)
	{
		if (Skill == Second)
		{
			ComboFirst = true;
		}
	}
	if (OldSkill == Second)
	{
		if (Skill == Third)
		{
			ComboLast = true;
		}
	}
}

void PlayerAttackState::Update()
{
	if (PlayerControl::GetInstance()->GetPlayer()->ModelSetJudg() == false)return;
	AttackCollision::GetInstance()->Update();
	
	ComboAction();
	HitStop();
	Player* l_player = PlayerControl::GetInstance()->GetPlayer();

	int DefaultDamage;
	if(l_player->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->FIRST)
	{
		DefaultDamage = 5;
	}
	if (l_player->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->SECOND)
	{
		DefaultDamage = 10;
	}
	if (l_player->GetAttackType() == PlayerControl::GetInstance()->GetPlayer()->THIRD)
	{
		DefaultDamage = 20;
	}
	AttackCollision::GetInstance()->GetCol(DefaultDamage+SelectSword::GetInstance()->GetSword()->GetDamage());
}

void PlayerAttackState::FirstAttack(std::vector<std::unique_ptr<Enemy>>& enemy)
{
	//2巡目のコンボ打消
	ComboLast = false;
	ComboFirst = false;
	//
	OldSkill = First;

	int randDam = rand() % 5 + 1;
	Damage = SelectSword::GetInstance()->GetSword()->GetDamage() + FIRSTATTACKDAMAGE + randDam;

	DetailAttack(enemy, 120);
}

void PlayerAttackState::SecondAttack(std::vector<std::unique_ptr<Enemy>>& enemy)
{
	OldSkill = Second;

	if (ComboFirst)
	{
		//コンボ時のダメージ
		Damage = SelectSword::GetInstance()->GetSword()->GetDamage() + SECONDATTACKDAMAGE + COMBOBONUS;
	}
	else
	{
		//通常ダメージ
		Damage = SelectSword::GetInstance()->GetSword()->GetDamage() + SECONDATTACKDAMAGE;
	}

	DetailAttack(enemy, 120);
	//}
}

void PlayerAttackState::ThirdAttack(std::vector<std::unique_ptr<Enemy>>& enemy)
{
	//
	OldSkill = Third;
	//1巡目のコンボ打消
	ComboFirst = false;

	if (ComboLast)
	{
		//コンボ時のダメージ
		Damage = SelectSword::GetInstance()->GetSword()->GetDamage() + ThirdAttackDamage + ComboBonus;
	}
	else
	{
		//通常のダメージ
		Damage = SelectSword::GetInstance()->GetSword()->GetDamage() + ThirdAttackDamage;
	}
	DetailAttack(enemy, 120);
}

void PlayerAttackState::SkillCoolDown(int& cooltime)
{
	cooltime--;
	cooltime = max(cooltime, 0);
	cooltime = min(cooltime, SelectSword::GetInstance()->GetSword()->GetCoolTime());
}

void PlayerAttackState::DetailAttack(std::vector<std::unique_ptr<Enemy>>& enemy, int cooltime)
{
	
	Skill = None;
}

#include"CameraControl.h"
void PlayerAttackState::SetHitStopJudg(bool f, int time)
{
	HitStopTime = time;
	HitStopJudg = f;
}
void PlayerAttackState::HitStop()
{
	if (HitStopJudg)
	{
		HitStopCount++;
		if (HitStopCount > HitStopTime)
		{
			HitStopJudg = false;
		}
		if (HitStopCount < 20)
		{
			CameraControl::GetInstance()->ShakeCamera();
		}
	}
	else
	{
		HitStopCount = 0;
	}
}
