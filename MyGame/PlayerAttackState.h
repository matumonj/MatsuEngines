
#include"Enemy.h"
#include"TargetMarker.h"
#include"Input.h"
class PlayerAttackState
{
public:

	void Update();

	enum AttackSkill
	{
		None,
		First,
		Second,
		Third,
		CoolDown,
		Buff,
	};

	//�����������Ă���X�L��
	AttackSkill Skill;
	//�����������Ă���X�L���̈�O�ɔ��������X�L�� �R���{�p
	AttackSkill OldSkill;
	int index;

	int Buffing();

	void BuffAction();
	//
	void FirstAttack(std::vector<std::unique_ptr<Enemy>>& enemy);
	//
	void SecondAttack(std::vector<std::unique_ptr<Enemy>>& enemy);
	//
	void ThirdAttack(std::vector<std::unique_ptr<Enemy>>& enemy);

	//�R���{1���� First->Second��
	void ComboAttackFirst();
	//�R���{2���� Second->Third��
	void ComboAttackLast();
	int GetDamage() { return Damage; }
	void ComboAction();
	void SkillCoolDown(int &cooltime);

	void DetailAttack(std::vector<std::unique_ptr<Enemy>>& enemy,int cooltime);
private:
	
	bool ComboFirst;
	bool ComboLast;

	int Damage;//�R���{�ňЗ͕ς�邽��const�̓i�V

	enum DamageValue
	{
		FIRSTATTACKDAMAGE = 2,
		SECONDATTACKDAMAGE = 105,
		THIRDAttackDamage = 100,
		COMBOBONUS = 10,
	};
	bool AttackJudgeMent = false;
	int FirstAttackDamage=100;
	int SecondAttackDamage=15;
	int ThirdAttackDamage=20;
	int ComboBonus = 10;
	int CoolDownTime = 0;

	int BufCoolDownTime = 0;

	int BuffTime=0;

	bool BuffFlag;
	const int BuffTimeMax = 1200;
	std::vector<std::unique_ptr<Enemy>>enemys;
public:
	static PlayerAttackState* GetInstance();
	AttackSkill GetSkill() { return OldSkill; }
	AttackSkill GetNowSkill() { return Skill; }
	int GetCoolTime() { return CoolDownTime; };

};
