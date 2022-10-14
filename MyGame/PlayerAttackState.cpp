
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
	index =  TargetMarker::GetInstance()->GetNearIndex();

//	if (AttackJudgeMent) {
		if (CustomButton::GetInstance()->GetAttackAction()==true ){ /*CoolDownTime == 0*/
			Skill = First;
		}
		if (CustomButton::GetInstance()->Get2AttackAction() == true) { /*CoolDownTime == 0*/
			Skill = Second;
		}
		if (CustomButton::GetInstance()->Get3AttackAction() == true) { /*CoolDownTime == 0*/
			Skill = Third;
		}
		/*else if (Input::GetInstance()->PushButton(Input::GetInstance()->Button_A) && CoolDownTime == 0) {
			Skill = Second;
		} else if (Input::GetInstance()->PushButton(Input::GetInstance()->Button_X) && CoolDownTime == 0) {
			Skill = Third;
		} else if (Input::GetInstance()->PushButton(Input::GetInstance()->Button_Y) && BufCoolDownTime == 0) {
			BuffFlag = true;
		}*/
	//}
	if (BuffFlag) {
		//�o�t�p������
		BuffTime++;
	}

	ComboAction();

	switch (Skill)
	{
	case PlayerAttackState::None:
		break;
	case PlayerAttackState::First:
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			FirstAttack(EnemyControl::GetInstance()->GetTutorialEnemyindex());
		}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
			FirstAttack(EnemyControl::GetInstance()->GetEnemyindex(0));
		}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
			FirstAttack(EnemyControl::GetInstance()->GetBossEnemyindex());
		}
		break;
	case PlayerAttackState::Second:
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			SecondAttack(EnemyControl::GetInstance()->GetTutorialEnemyindex());
		}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
			FirstAttack(EnemyControl::GetInstance()->GetEnemyindex(0));
		}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
			FirstAttack(EnemyControl::GetInstance()->GetBossEnemyindex());
		}
		//SecondAttack(EnemyControl::GetInstance()->GetEnemyindex(1)
		//);
		break;
	case PlayerAttackState::Third:
		if (SceneManager::GetInstance()->GetScene() == SceneManager::TUTORIAL) {
			ThirdAttack(EnemyControl::GetInstance()->GetTutorialEnemyindex());
		}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::PLAY) {
			FirstAttack(EnemyControl::GetInstance()->GetEnemyindex(0));
		}
		if (SceneManager::GetInstance()->GetScene() == SceneManager::BOSS) {
			FirstAttack(EnemyControl::GetInstance()->GetBossEnemyindex());
		}
		//ThirdAttack(EnemyControl::GetInstance()->GetEnemyindex(1)
		//);
		break;
	}
	//�X�L���N�[���_�E������
	SkillCoolDown(CoolDownTime);
	
	//if (PlayerControl::GetInstance()->GetPlayer()->GetFbxTime() >= 1.7f) {
		AttackCollision::GetInstance()->GetCol(Damage);
//	}

}

void PlayerAttackState::FirstAttack(std::vector<std::unique_ptr<Enemy>>& enemy)
{
	//2���ڂ̃R���{�ŏ�
	ComboLast = false;
	//
	OldSkill = First;
	
	int randDam = rand() % 5+1;
	Damage = SelectSword::GetInstance()->GetSword()->GetDamage()+FIRSTATTACKDAMAGE+randDam;

	DetailAttack(enemy,120);
}

void PlayerAttackState::SecondAttack(std::vector<std::unique_ptr<Enemy>>& enemy)
{
	OldSkill = Second;
	
	if (ComboFirst) {//�R���{���̃_���[�W
		Damage = SelectSword::GetInstance()->GetSword()->GetDamage() + SECONDATTACKDAMAGE+COMBOBONUS;
	}else {//�ʏ�_���[�W
		Damage = SelectSword::GetInstance()->GetSword()->GetDamage() + SECONDATTACKDAMAGE ;
	}

	DetailAttack(enemy,120);
//}
}

void PlayerAttackState::ThirdAttack(std::vector<std::unique_ptr<Enemy>>& enemy)
{
	//
	OldSkill = Third;
	//1���ڂ̃R���{�ŏ�
	ComboFirst = false;

	if (ComboLast) {//�R���{���̃_���[�W
		Damage = SelectSword::GetInstance()->GetSword()->GetDamage() + ThirdAttackDamage+ComboBonus;
	} else {//�ʏ�̃_���[�W
		Damage = SelectSword::GetInstance()->GetSword()->GetDamage() + ThirdAttackDamage;
	}
	DetailAttack(enemy,120);
}


void PlayerAttackState::BuffAction()
{
	//���ʂ̍U����25%���o�t�l��
	const int Magnification = Damage/4;

		if (BuffTime <= BuffTimeMax) {//���ʎ��Ԃ�20�b
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
	cooltime = min(cooltime, 120);
}

void PlayerAttackState::DetailAttack(std::vector<std::unique_ptr<Enemy>>& enemy,int cooltime)
{
	//if (enemy[0] == nullptr)return;
	//�N�[���^�C����ݒ�
	CoolDownTime = SelectSword::GetInstance()->GetSword()->GetCoolTime();
	//�o�t���������Ă����ꍇ
	if (BuffFlag) {
		BuffAction();
	}
	
	Skill = None;
}


