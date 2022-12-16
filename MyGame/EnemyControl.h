#include "ControlBase.h"
#include<fstream>
#include<string>
#include<sstream>
#include<ostream>
#include <iostream>
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

	std::vector<std::vector<std::unique_ptr<Enemy>>> enemys = {};

	XMFLOAT3 tutorial_pos = { 0,0,0 };
	XMFLOAT3 boss_pos = { 0,0,0 };

	Enemy* encountEnemy = nullptr;
private:
	enum
	{
		ALPHAENEMY = 1,
		BETAENEMY = 2,
	};

public:
	static EnemyControl* GetInstance();

	void Load(DebugCamera* camera) override;

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
	static constexpr int EnemySize = 2;
	std::array<std::unique_ptr<Enemy>, EnemySize>SummonEnemys = {};
	bool summonEnemyCreate = false;
	XMFLOAT3 SummonEPos = { 1,1,1 };
	bool SummonEnemysDeath;
	bool SummonEnemysApper;
	float Shieldalpha;

private:
	bool ShieldCreate;
	bool ShieldLost;
	std::unique_ptr<Texture>ShieldTex[4];
	float Texangle[4];
	std::array<XMFLOAT3, 4>ShieldTexPos;
	float ShieldTexAlpha;
	std::unique_ptr<BomAttack>bAttack;
	
private:
	/*更新処理*/
	void Init_Tutorial(DebugCamera* camera) override;

	void Init_Play(DebugCamera* camera) override;

	void Init_Boss(DebugCamera* camera) override;

	/*更新処理*/
	void Update_Tutorial(DebugCamera* camera) override;

	void Update_Play(DebugCamera* camera) override;

	void Update_Boss(DebugCamera* camera) override;

	/*更新処理*/
	void Draw_Tutorial() override;

	void Draw_Play() override;

	void Draw_Boss() override;

	void SummonEnemyInit(DebugCamera* camera);
	void SummonEnemyUpdate(DebugCamera* camera);
public:
	bool GetSummonEnemysDeath() { return SummonEnemysDeath; }

	bool GetSummonEnemysApper() { return SummonEnemysApper; }
	Enemy* GetSummonEnemy(int index) { return SummonEnemys[index].get(); }
};
