#include "ControlBase.h"
#include"Enemy.h"
#include"DebugCamera.h"
#include<memory>
#include<array>

#include "CircleAttack.h"

class EnemyControl :
	public ControlBase
{
public:
	~EnemyControl() override
	{
	};
private:
	std::vector<XMFLOAT3> Load_EnemyPosition = {};

	std::vector<int>rescount = {};
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
	static EnemyControl* GetInstance();

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

private:
	static constexpr int EnemySize = 2;
	std::array<std::unique_ptr<Enemy>, EnemySize> SummonEnemys = {};
	std::unique_ptr<Enemy> Guardian;
	bool summonEnemyCreate = false;
	XMFLOAT3 SummonEPos = {1, 1, 1};
	bool SummonEnemysDeath;
	bool SummonEnemysApper;
	float Shieldalpha;

public:
	void GuardianCreate();
	void GuardianReset();
private:
	bool ShieldCreate;
	bool ShieldLost;
	std::unique_ptr<Texture> ShieldTex[4];
	float Texangle[4];
	std::array<XMFLOAT3, 4> ShieldTexPos;
	float ShieldTexAlpha;
	std::unique_ptr<BomAttack> bAttack;

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

	bool GetSummonEnemysDeath() { return SummonEnemysDeath; }

	bool GetSummonEnemysApper() { return SummonEnemysApper; }
	Enemy* GetSummonEnemy(int index) { return SummonEnemys[index].get(); }

public:
	void GameOverResetParam();
};
