#pragma once
#include"Object3d.h"
#include<memory>
#include<array>
#include"Particle.h"
#include"CollisionPrimitive.h"
class BossMap
{
private:

	enum Map {
		NONE,
		BLOCK,
		DAMAGEBLOCK
	};
	static constexpr int mapWidth =17;
	static constexpr int mapHight= 17;

	int mapSize[mapHight][mapWidth];

	static constexpr float BlockSize = 20;

	std::unique_ptr<Object3d>mapobj[mapHight][mapWidth];
	std::unique_ptr<Object3d>nailobj[mapHight][mapWidth];
	std::unique_ptr<Particle>par[mapHight][mapWidth];
	DirectX::XMFLOAT3 nailPos[mapHight][mapWidth];
	float nailalpha[mapHight][mapWidth] = { 1 };

	DirectX::XMFLOAT3 cpos = {-200,11.6,-200};
	DirectX::XMFLOAT3 blockColor[mapHight][mapWidth];
	float blockColorETime[mapHight][mapWidth];

	DirectX::XMFLOAT3 bsize = {10,4,10};
	int rockappertime;
	float rockalpha=1;

	Point bpoint[mapHight][mapWidth];
	Line2D RushArea;

	Line2D DamageLine;
	bool damageLineDrawFlag;
	DirectX::XMFLOAT2 lineendpos;

	static constexpr int lanthanumSize = 12;

	std::array<std::unique_ptr<Object3d>, lanthanumSize>Lanthanum;
	std::array<DirectX::XMFLOAT3, lanthanumSize>LanthanumPos;
	std::array<DirectX::XMFLOAT3, lanthanumSize>LanthanumScl;
public:
	static BossMap* GetInstance();
	void Init();
	void Upda();
	void Draw();

	void SetDamageArea(Line2D damageline, bool atckjudg) { if (atckjudg){DamageLine = damageline; } damageLineDrawFlag = atckjudg;};

	void DamageBlockMove();
	enum phase {
		NON,
		INI,
		UPDA,
		END
	};
	phase p;
	void SetDamageArea();
public:
	void DrawDamageLine(bool atckjudg,Line2D line);
	void DrawDamageLine(bool atckjudg, Line2D line[4]);
};

