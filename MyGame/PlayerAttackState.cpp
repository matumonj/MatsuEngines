
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
PlayerAttackState* PlayerAttackState::GetInstance()
{
	static PlayerAttackState instance;

	return &instance;
}

void PlayerAttackState::ComboAction()
{
	if (OldSkill == First) {
		if (Skill == Second) {
			ComboFirst = true;
		}
	}
	if (OldSkill == Second) {
		if (Skill == Third) {
			ComboLast = true;
		}
	}
	
}
void PlayerAttackState::Update()
{
	AttackCollision::GetInstance()->Update();

	if (CustomButton::GetInstance()->GetAttackAction()==true &&CoolDownTime==0){ /*CoolDownTime == 0*/
			Skill = First;
		}
		if (CustomButton::GetInstance()->Get2AttackAction() == true && CoolDownTime == 0) { /*CoolDownTime == 0*/
			Skill = Second;
		}
		if (CustomButton::GetInstance()->Get3AttackAction() == true && CoolDownTime == 0) { /*CoolDownTime == 0*/
			Skill = Third;
		}
		
	if (BuffFlag) {
		//バフ継続時間
		BuffTime++;
	}

	ComboAction();

	switch (Skill)
	{
	case PlayerAttackState::None:
		break;
	case PlayerAttackState::First:
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			FirstAttack(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL));
		}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
			FirstAttack(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE));
		}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
			FirstAttack(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS));
		}
		break;
	case PlayerAttackState::Second:
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			SecondAttack(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL));
		}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
			SecondAttack(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE));
		}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
			SecondAttack(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS));
		}
		//SecondAttack(EnemyControl::GetInstance()->GetEnemyindex(1)
		//);
		break;
	case PlayerAttackState::Third:
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			ThirdAttack(EnemyControl::GetInstance()->GetEnemy(EnemyControl::TUTORIAL));
		}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
			FirstAttack(EnemyControl::GetInstance()->GetEnemy(EnemyControl::PLAYSCENE));
		}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
			FirstAttack(EnemyControl::GetInstance()->GetEnemy(EnemyControl::BOSS));
		}
		//ThirdAttack(EnemyControl::GetInstance()->GetEnemyindex(1)
		//);
		break;
	}
	//スキルクールダウン処理
	SkillCoolDown(CoolDownTime);
	HitStop();
	//if (PlayerControl::GetInstance()->GetPlayer()->GetFbxTime() >= 1.7f) {
		AttackCollision::GetInstance()->GetCol(Damage);
//	}

}

void PlayerAttackState::FirstAttack(std::vector<std::unique_ptr<Enemy>>& enemy)
{
	//2巡目のコンボ打消
	ComboLast = false;
	ComboFirst = false;
	//
	OldSkill = First;
	
	int randDam = rand() % 5+1;
	Damage = SelectSword::GetInstance()->GetSword()->GetDamage()+FIRSTATTACKDAMAGE+randDam;

	DetailAttack(enemy,120);
}

void PlayerAttackState::SecondAttack(std::vector<std::unique_ptr<Enemy>>& enemy)
{
	OldSkill = Second;
	
	if (ComboFirst) {//コンボ時のダメージ
		Damage = SelectSword::GetInstance()->GetSword()->GetDamage() + SECONDATTACKDAMAGE+COMBOBONUS;
	}else {//通常ダメージ
		Damage = SelectSword::GetInstance()->GetSword()->GetDamage() + SECONDATTACKDAMAGE ;
	}

	DetailAttack(enemy,120);
//}
}

void PlayerAttackState::ThirdAttack(std::vector<std::unique_ptr<Enemy>>& enemy)
{
	//
	OldSkill = Third;
	//1巡目のコンボ打消
	ComboFirst = false;

	if (ComboLast) {//コンボ時のダメージ
		Damage = SelectSword::GetInstance()->GetSword()->GetDamage() + ThirdAttackDamage+ComboBonus;
	} else {//通常のダメージ
		Damage = SelectSword::GetInstance()->GetSword()->GetDamage() + ThirdAttackDamage;
	}
	DetailAttack(enemy,120);
}


void PlayerAttackState::BuffAction()
{
	//普通の攻撃の25%をバフ値に
	const int Magnification = Damage/4;

		if (BuffTime <= BuffTimeMax) {//効果時間は20秒
			if (Skill != None) {
				Damage += Magnification;
			}
		} else if(BuffTime>BuffTimeMax){
			BuffTime = 0;
			BuffFlag = false;
		}
}

void PlayerAttackState::SkillCoolDown(int &cooltime)
{
	cooltime--;
	cooltime= max(cooltime, 0);
	cooltime = min(cooltime, SelectSword::GetInstance()->GetSword()->GetCoolTime());
}

void PlayerAttackState::DetailAttack(std::vector<std::unique_ptr<Enemy>>& enemy,int cooltime)
{
	//if (enemy[0] == nullptr)return;
	//クールタイムを設定
	CoolDownTime = SelectSword::GetInstance()->GetSword()->GetCoolTime();
	//バフがかかっていた場合
	if (BuffFlag) {
		BuffAction();
	}
	
	Skill = None;
}

#include"CameraControl.h"
void PlayerAttackState::HitStop()
{
	if (HitStopJudg) {
		HitStopTime++;
		if (HitStopTime > 30) {
			HitStopJudg = false;
		}
		if (HitStopTime < 2) {
			CameraControl::GetInstance()->ShakeCamera();
		}
	}
	else {
		HitStopTime = 0;
	}
	//
}