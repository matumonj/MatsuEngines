#include"Enemy.h"
#include"TargetMarker.h"
#include"Input.h"

class PlayerAttackState
{
public:
	void Update();

	void Finalize();

	enum AttackSkill
	{
		None,
		First,
		Second,
		Third,
		CoolDown,
		Buff,
	};

	//今発動させているスキル
	AttackSkill Skill;
	//今発動させているスキルの一つ前に発動したスキル コンボ用
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

	//コンボ1巡目 First->Second時
	void ComboAttackFirst();
	//コンボ2巡目 Second->Third時
	void ComboAttackLast();
	int GetDamage() { return Damage; }
	void ComboAction();
	void SkillCoolDown(int& cooltime);

	void DetailAttack(std::vector<std::unique_ptr<Enemy>>& enemy, int cooltime);
private:
	bool ComboFirst;
	bool ComboLast;

	int Damage; //コンボで威力変わるためconstはナシ

	enum DamageValue
	{
		FIRSTATTACKDAMAGE = 2,
		SECONDATTACKDAMAGE = 7,
		THIRDAttackDamage = 7,
		COMBOBONUS = 5,
	};

	bool AttackJudgeMent = false;
	int FirstAttackDamage = 100;
	int SecondAttackDamage = 15;
	int ThirdAttackDamage = 20;
	int ComboBonus = 10;
	int HitStopTime;
	int HitStopCount;
	int HitStopJudg;
	
	bool BuffFlag;
	const int BuffTimeMax = 1200;
	std::vector<std::unique_ptr<Enemy>> enemys;
public:
	static PlayerAttackState* GetIns();
	AttackSkill GetSkill() { return OldSkill; }
	AttackSkill GetNowSkill() { return Skill; }
	void SetHitStopJudg(bool f, int time = 90);
	bool GetHitStopJudg() { return HitStopJudg; }
	void HitStop();
};
