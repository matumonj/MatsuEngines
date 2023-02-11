#pragma once
#include"Object3d.h"
#include<memory>
#include<array>
#include"Particle.h"
#include"CollisionPrimitive.h"

class BossMap
{
public:
	~BossMap();
private:
	enum Map
	{
		NONE,
		BLOCK,
		DAMAGEBLOCK
	};

	static constexpr int mapWidth = 15;
	static constexpr int mapHight = 15;

	int mapSize[mapHight][mapWidth];

	static constexpr float BlockSize = 20.f;

	std::unique_ptr<Object3d> mapobj[mapHight][mapWidth];
	std::unique_ptr<Object3d> nailobj[mapHight][mapWidth];
	std::unique_ptr<Particle> par[mapHight][mapWidth];
	DirectX::XMFLOAT3 nailPos[mapHight][mapWidth];
	float blockalpha[mapHight][mapWidth] = {1};

	DirectX::XMFLOAT3 cpos = {-150.f, 11.6f, -150.f};
	DirectX::XMFLOAT3 blockColor[mapHight][mapWidth];
	float blockColorETime[mapHight][mapWidth];

	DirectX::XMFLOAT3 bsize = {10.f, 4.f, 10.f};
	int rockappertime;
	float rockalpha = 1.f;

	Point bpoint[mapHight][mapWidth];
	Line2D RushArea;

	Line2D DamageLine;
	bool damageLineDrawFlag;
	DirectX::XMFLOAT2 lineendpos;
public:
	static BossMap* GetIns();
	void Init();
	void Upda();
	void Draw();

	void SetDamageArea(Line2D damageline, bool atckjudg)
	{
		if (atckjudg) { DamageLine = damageline; }
		damageLineDrawFlag = atckjudg;
	};

	void DamageBlockMove();

	enum phase
	{
		NON,
		INI,
		UPDA,
		END
	};

	phase p;
	void SetDamageArea();
public:
	void DrawDamageLine(bool atckjudg, Line2D line);
	void DrawDamageLine(bool atckjudg, Line2D line[4]);
};
