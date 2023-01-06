#include "PlayerAttackState.h"
#include"Input.h"
#include"EnemyControl.h"
#include"Collision.h"
#include"CustomButton.h"
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
	AttackCollision::GetInstance()->Update();

	if (CustomButton::GetInstance()->GetAttackAction() == true && CoolDownTime == 0)
	{
		/*CoolDownTime == 0*/
		Skill = First;
	}

	if (BuffFlag)
	{
		//バフ継続時間
		BuffTime++;
	}

	ComboAction();

	//スキルクールダウン処理
	SkillCoolDown(CoolDownTime);
	HitStop();
	AttackCollision::GetInstance()->GetCol(SelectSword::GetInstance()->GetSword()->GetDamage());
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


void PlayerAttackState::BuffAction()
{
	//普通の攻撃の25%をバフ値に
	const int Magnification = Damage / 4;

	if (BuffTime <= BuffTimeMax)
	{
		//効果時間は20秒
		if (Skill != None)
		{
			Damage += Magnification;
		}
	}
	else if (BuffTime > BuffTimeMax)
	{
		BuffTime = 0;
		BuffFlag = false;
	}
}

void PlayerAttackState::SkillCoolDown(int& cooltime)
{
	cooltime--;
	cooltime = max(cooltime, 0);
	cooltime = min(cooltime, SelectSword::GetInstance()->GetSword()->GetCoolTime());
}

void PlayerAttackState::DetailAttack(std::vector<std::unique_ptr<Enemy>>& enemy, int cooltime)
{
	//if (enemy[0] == nullptr)return;
	//クールタイムを設定
	CoolDownTime = SelectSword::GetInstance()->GetSword()->GetCoolTime();
	//バフがかかっていた場合
	if (BuffFlag)
	{
		BuffAction();
	}

	Skill = None;
}

#include"CameraControl.h"

void PlayerAttackState::HitStop()
{
	if (HitStopJudg)
	{
		HitStopTime++;
		if (HitStopTime > 60)
		{
			HitStopJudg = false;
		}
		if (HitStopTime < 20)
		{
			CameraControl::GetInstance()->ShakeCamera();
		}
	}
	else
	{
		HitStopTime = 0;
	}
}
