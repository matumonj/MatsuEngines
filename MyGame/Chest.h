#pragma once
#include "ObjectManager.h"
#include"ParticleManager.h"

class Chest :
	public ObjectManager
{
public:
	Chest()
	{
	};
	~Chest() override;

public:
	void Initialize(DebugCamera* camera) override;
	void Update(DebugCamera* camera) override;
	void Draw() override;
	bool CollideChest();

public:
	void SetChestLost(bool f) { ChestLost = f; }
	void SetpColor(XMFLOAT4 color) { pColor = color; }
private:
	XMFLOAT4 pColor = {1,1,1,1};
	bool ChestLost=false;
	ParticleManager* DefaultEffect=nullptr;
};
