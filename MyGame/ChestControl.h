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
	enum Event {
		FEEDIN,
		FEEDOUT,
		GETCHEST,
		FEEDIN2,
		FEEDOUT2,
		END,
		NON
	};
	
private:
	std::vector<std::unique_ptr<Chest>>chests;
	std::vector <XMFLOAT3>Load_ChestPosition;
	std::vector<std::unique_ptr<Chest>>Tutorial_chest;
	int GetChestCount = 0;
	bool GetTutorialChestJudg;
	int ChestMax;
private:
	struct ParticleParam {
		ParticleManager* particleMan = nullptr;
		bool ParticleCreateF;
		XMFLOAT3 particlePos;
		int ParticleLife = 420;
		int pCount;
		XMFLOAT3 vel{};
		XMFLOAT3 acc{};
	};
	Event ChestEvent = NON;
	ParticleParam TutorialPchest;
	ParticleParam PlayPchest[4];
	void GetChestEffect(Chest* chest, ParticleParam pParam);
	void GetChestEvent(Chest* chest, ParticleParam pParam);

};

