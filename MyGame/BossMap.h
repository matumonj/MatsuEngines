#pragma once
#include"Object3d.h"
#include<memory>
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
	DirectX::XMFLOAT3 bsize = {10,4,10};
	int rockappertime;
	float rockalpha=1;

	Point bpoint[mapHight][mapWidth];
	Line2D RushArea;

	DirectX::XMFLOAT2 lineendpos;
public:
	static BossMap* GetInstance();
	void Init();
	void Upda();
	void Draw();

	void DamageBlockMove();
	enum phase {
		NON,
		INI,
		UPDA,
		END
	};
	phase p;
	void SetDamageArea();
};

