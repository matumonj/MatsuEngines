#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Input.h"
#include"CollisionPrimitive.h"

class AttackCollision
{
public:
	static AttackCollision* GetIns();
private:
	template <class T>
	using ComPtr = ComPtr<T>;
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
private:
	bool colf;
	bool setf;
	bool boss_colf;
	std::vector<bool> Play_colf;
	bool HitCol;
	bool HitCol2;
	OBB HandObb;
	bool attackCol[2];
	//OBBCollision* ColObb;
	//OBBCollision* BossColObb;
	//std::vector<OBBCollision*>PlayColOBB;
	std::vector<Box> EnemyArea;
	Input* input;

	bool CameraZoom;
public:
	void Init();

	void Update();

	void GetCol(int damage);

	void Draw();

	void Finalize();

	bool GetCameraZoom() { return CameraZoom; }
	bool GetHelpJudg() { return HelpJudg; }
private:
	bool HelpJudg;

	bool attackcolJudgTime_First;
	bool attackcolJudgTime_Second;
	bool attackcolJudgTime_Third;

	void TutorialCol(int damage);
	void ExplorationCol(int damage);
	void BossCol(int damage);
private:
	enum ColType
	{
		TYTORIAL,
		PLAY,
		BOSS
	};

	std::vector<OBB> EnemyOBB;
	std::vector<OBB> PlayEnemyOBB;
	std::vector<OBB> BossEnemyOBB;
	OBB GuardianEnemyOBB;
	OBB SummonEnemyOBB[2];
	void OBBParamSet(ColType Enemytype);
};
