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
	void Initialize() override;
	void Update() override;
	void Draw() override;
	bool CollideChest();

public:
	void SetChestLost(bool f) { ChestLost = f; }
	void SetpColor(XMFLOAT4 color) { pColor = color; }
private:
	XMFLOAT4 pColor;
	bool ChestLost;
	ParticleManager* DefaultEffect;
};
