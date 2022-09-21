#pragma once
#include"Object3d.h"
#include"Model.h"
#include"Input.h"
#include"CollisionPrimitive.h"
#include"OBB.h"
#include"OBBCollision.h"
class AttackCollision
{
public:
	static AttackCollision* GetInstance();
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
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
	std::vector<bool>Play_colf;
	OBB HandObb;
	OBBCollision* ColObb;
	OBBCollision* BossColObb;
	std::vector<OBBCollision*>PlayColOBB;
	std::vector<Box> EnemyArea;
	Input* input;
public:
	void Init();

	void Update();

	void GetCol(int damage);

	void Draw();

	void Finalize();
private:
	enum ColType{
		TYTORIAL,
		PLAY,
		BOSS
	};
	std::vector<OBB>EnemyOBB;
	std::vector<OBB>PlayEnemyOBB;
	std::vector<OBB>BossEnemyOBB;
	void ColOBB(ColType Enemytype);
};

