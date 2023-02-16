#include "ControlBase.h"
#include"Enemy.h"
#include<memory>
#include<array>

#include "CircleAttack.h"
#include "Particle.h"

class EnemyControl :
	public ControlBase
{
public:
	~EnemyControl() override;
private:
	std::vector<XMFLOAT3> Load_EnemyPosition = {};

	std::vector<int> rescount = {};
	std::vector<std::vector<std::unique_ptr<Enemy>>> enemys = {};

	XMFLOAT3 tutorial_pos = {0, 0, 0};
	XMFLOAT3 boss_pos = {0, 0, 0};

	Enemy* encountEnemy = nullptr;
private:
	enum
	{
		GOLEMENEMY = 1,
		ALPHAENEMY = 2,
		BETAENEMY=3
	};

public:
	static EnemyControl* GetIns();

	void Load() override;

	void HPFrameDraw();

	void Finalize() override;

	enum EnemyType
	{
		NON,
		TUTORIAL,
		PLAYSCENE,
		BOSS,
	};

	int index = 0;
	std::vector<std::unique_ptr<Enemy>>& GetEnemy(EnemyType type);

	
	Enemy* GetEncountEnemy()
	{
		if (encountEnemy != nullptr)
		{
			return encountEnemy;
		}
	}

public:
	void GuardianCreate();
	void GuardianReset();
private:
	std::unique_ptr<Enemy> Guardian;
private:
	/*更新処理*/
	void Init_Tutorial() override;

	void Init_Play() override;

	void Init_Boss() override;

	/*更新処理*/
	void Update_Tutorial() override;

	void Update_Play() override;

	void Update_Boss() override;

	/*更新処理*/
	void Draw_Tutorial() override;

	void Draw_Play() override;

	void Draw_Boss() override;

	void SummonEnemyInit();
	void SummonEnemyUpdate();

public:
	Enemy* GetGuardianEnemy() { return Guardian.get(); }

	void GameOverResetParam();
};
