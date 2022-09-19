#pragma once
#include"ControlBase.h"
#include"Chest.h"
#include"ParticleManager.h"
class ChestControl :public ControlBase
{
public:
	static ChestControl* GetInstance();

	void Load(DebugCamera* camera)override;

	void Initialize(DebugCamera* camera)override;

	void Update(DebugCamera* camera)override;

	void Draw()override;

	void Finalize()override;

public:
	void SetColor(XMFLOAT4 color) {};

	void GetChestAction();

	void ChestDestroy();

	bool GetTutorialChest() { return GetTutorialChestJudg; }

	int ChestCount() { return GetChestCount; }
private:
	void GetChestEvent(Chest* chest);
	enum Event {
		FEEDIN,
		FEEDOUT,
		GETCHEST,
		FEEDIN2,
		FEEDOUT2,
		END,
		NON
	};
	Event ChestEvent = NON;
private:
	std::vector<std::unique_ptr<Chest>>chests;
	std::vector <XMFLOAT3>Load_ChestPosition;
	std::vector<std::unique_ptr<Chest>>Tutorial_chest;
	int GetChestCount = 0;
	bool GetTutorialChestJudg;
	int ChestMax;
private:
	ParticleManager* particleMan = nullptr;
	bool ParticleCreateF;
	XMFLOAT3 particlePos;
	int ParticleLife = 420;
	int pCount;
	void GetChestEffect(Chest*chest);
};

